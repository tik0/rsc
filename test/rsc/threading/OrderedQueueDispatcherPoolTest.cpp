/* ============================================================
 *
 * This file is a part of RSC project
 *
 * Copyright (C) 2010 by Johannes Wienke <jwienke at techfak dot uni-bielefeld dot de>
 *
 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General
 * Public License as published by the Free Software Foundation;
 * either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * ============================================================ */

#include <stdexcept>
#include <ostream>
#include <vector>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread.hpp>

#include <gtest/gtest.h>

#include "rsc/threading/OrderedQueueDispatcherPool.h"

using namespace std;
using namespace rsc;
using namespace rsc::threading;

class StubReceiver {
public:
    static unsigned int nextReceiverNum;
    unsigned int receiverNum;
    StubReceiver() :
        receiverNum(nextReceiverNum++) {
    }
    boost::condition condition;
    boost::mutex mutex;
    vector<int> messages;
};

unsigned int StubReceiver::nextReceiverNum = 1;

ostream &operator<<(ostream &stream, const StubReceiver &receiver) {
    return stream << "StubReceiver[" << receiver.receiverNum << "]";
}

void deliver(boost::shared_ptr<StubReceiver> &receiver, const int &message) {
    boost::mutex::scoped_lock lock(receiver->mutex);

    // emulate slow processing
    boost::this_thread::sleep(
            boost::posix_time::millisec(rand() % (receiver->receiverNum * 50)));

    receiver->messages.push_back(message);
    receiver->condition.notify_all();

}

/**
 * A delivery handler based on the object-oriented interface.
 *
 * @author jwienke
 */
class DeliveryHandler: public OrderedQueueDispatcherPool<int, StubReceiver>::DeliveryHandler {
public:
    void deliver(boost::shared_ptr<StubReceiver> &receiver, const int &message) {
        boost::mutex::scoped_lock lock(receiver->mutex);
        receiver->messages.push_back(message);
        receiver->condition.notify_all();
    }
};

TEST(OrderedQueueDispatcherPoolTest, testProcessing)
{

    StubReceiver::nextReceiverNum = 1;

    srand(time(NULL));

    const unsigned int numMessages = 100;
    OrderedQueueDispatcherPool<int, StubReceiver> pool(4,
            boost::bind(deliver, _1, _2));

    const unsigned int numReceivers = 11;
    vector<boost::shared_ptr<StubReceiver> > receivers;
    for (unsigned int i = 0; i < numReceivers; ++i) {
        boost::shared_ptr<StubReceiver> r(new StubReceiver);
        pool.registerReceiver(r);
        receivers.push_back(r);
    }

    pool.start();
    EXPECT_THROW(pool.start(), ::rsc::misc::IllegalStateException);

    // start jobs
    for (unsigned int i = 0; i < numMessages; ++i) {
        pool.push(i);
    }

    // wait for processing
    for (unsigned int i = 0; i < receivers.size(); ++i) {
        boost::mutex::scoped_lock lock(receivers[i]->mutex);
        while (receivers[i]->messages.size() < numMessages) {
            receivers[i]->condition.wait(lock);
        }
    }

    pool.stop();

    for (unsigned int i = 0; i < numReceivers; ++i) {

        boost::shared_ptr<StubReceiver> r = receivers[i];
        EXPECT_EQ(numMessages, r->messages.size());

        for (unsigned int expected = 0; expected < numMessages; ++expected) {
            EXPECT_EQ((int) expected, r->messages[expected]) << "Receiver " << i << " unordered";
        }

    }

}

TEST(OrderedQueueDispatcherPoolTest, testProcessingObjectOrientedInterface)
{

    StubReceiver::nextReceiverNum = 1;

    srand(time(NULL));

    const unsigned int numMessages = 100;
    OrderedQueueDispatcherPool<int, StubReceiver> pool(
            4,
            OrderedQueueDispatcherPool<int, StubReceiver>::DeliveryHandlerPtr(
                    new DeliveryHandler));

    const unsigned int numReceivers = 11;
    vector<boost::shared_ptr<StubReceiver> > receivers;
    for (unsigned int i = 0; i < numReceivers; ++i) {
        boost::shared_ptr<StubReceiver> r(new StubReceiver);
        pool.registerReceiver(r);
        receivers.push_back(r);
    }

    pool.start();
    EXPECT_THROW(pool.start(), ::rsc::misc::IllegalStateException);

    // start jobs
    for (unsigned int i = 0; i < numMessages; ++i) {
        pool.push(i);
    }

    // wait for processing
    for (unsigned int i = 0; i < receivers.size(); ++i) {
        boost::mutex::scoped_lock lock(receivers[i]->mutex);
        while (receivers[i]->messages.size() < numMessages) {
            receivers[i]->condition.wait(lock);
        }
    }

    pool.stop();

    for (unsigned int i = 0; i < numReceivers; ++i) {

        boost::shared_ptr<StubReceiver> r = receivers[i];
        EXPECT_EQ(numMessages, r->messages.size());

        for (unsigned int expected = 0; expected < numMessages; ++expected) {
            EXPECT_EQ((int) expected, r->messages[expected]) << "Receiver " << i << " unordered";
        }

    }

}

void laterRegister(boost::shared_ptr<StubReceiver> lateReceiver,
        boost::shared_ptr<StubReceiver> earlyUnregisteredReceiver,
        boost::shared_ptr<StubReceiver> observingReceiver,
        OrderedQueueDispatcherPool<int, StubReceiver> *pool,
        const unsigned int &numMessages) {

    // wait until the observer got 50% of the messages, then register the new
    // receiver
    boost::mutex::scoped_lock lock(observingReceiver->mutex);
    while (observingReceiver->messages.size() != (numMessages) / 2) {
        observingReceiver->condition.wait(lock);
    }
    pool->registerReceiver(lateReceiver);
    bool unregistred = pool->unregisterReceiver(earlyUnregisteredReceiver);
    assert(unregistred);

    // add remaining messages to the pool
    for (unsigned int i = 0; i < (numMessages / 2); ++i) {
        pool->push(i + (numMessages / 2));
    }

}

TEST(OrderedQueueDispatcherPoolTest, testRegistrationWhileProcessing)
{

    StubReceiver::nextReceiverNum = 1;

    srand(time(NULL));

    const unsigned int numMessages = 100;
    OrderedQueueDispatcherPool<int, StubReceiver> pool(2,
            boost::bind(deliver, _1, _2));

    const unsigned int numReceivers = 5;
    vector<boost::shared_ptr<StubReceiver> > receivers;
    for (unsigned int i = 0; i < numReceivers; ++i) {
        boost::shared_ptr<StubReceiver> r(new StubReceiver);
        pool.registerReceiver(r);
        receivers.push_back(r);
    }

    boost::shared_ptr<StubReceiver> lateReceiver(new StubReceiver);
    boost::shared_ptr<StubReceiver> earlyUnregisteredReceiver(new StubReceiver);
    pool.registerReceiver(earlyUnregisteredReceiver);
    boost::thread registerThread(
            boost::bind(laterRegister, lateReceiver, earlyUnregisteredReceiver,
                    receivers[0], &pool, numMessages));

    // add jobs
    for (unsigned int i = 0; i < (numMessages / 2); ++i) {
        pool.push(i);
    }

    // test starting after adding jobs
    pool.start();

    // wait for processing
    for (unsigned int i = 0; i < receivers.size(); ++i) {
        boost::mutex::scoped_lock lock(receivers[i]->mutex);
        while (receivers[i]->messages.size() < numMessages) {
            receivers[i]->condition.wait(lock);
        }
    }
    {
        boost::mutex::scoped_lock lock(lateReceiver->mutex);
        while (lateReceiver->messages.size() > 0
                && lateReceiver->messages.back() == (int) numMessages) {
            lateReceiver->condition.wait(lock);
        }
    }

    // check normal observers
    for (unsigned int i = 0; i < numReceivers; ++i) {

        boost::shared_ptr<StubReceiver> r = receivers[i];
        EXPECT_EQ(numMessages, r->messages.size());

        for (unsigned int expected = 0; expected < numMessages; ++expected) {
            EXPECT_EQ((int) expected, r->messages[expected]) << "Receiver " << i << " unordered";
        }

    }
    // check later registered receiver
    EXPECT_GT(earlyUnregisteredReceiver->messages.size(), (size_t) 0);
    EXPECT_LE(earlyUnregisteredReceiver->messages.size(), numMessages / 2);
    // check early unregistered receiver
    EXPECT_GT(earlyUnregisteredReceiver->messages.size(), (size_t) 0);
    EXPECT_LT(earlyUnregisteredReceiver->messages.size(), numMessages);

}

static boost::mutex rejectMutex;
static boost::condition rejectCondition;
static unsigned int rejectCalls = 0;

bool rejectFilter(boost::shared_ptr<StubReceiver> &/*receiver*/, const int &/*message*/) {
    boost::mutex::scoped_lock lock(rejectMutex);
    ++rejectCalls;
    rejectCondition.notify_all();
    return false;
}

class RejectFilterHandler: public OrderedQueueDispatcherPool<int, StubReceiver>::FilterHandler {
public:

    unsigned int rejectCalls;
    boost::mutex rejectMutex;
    boost::condition rejectCondition;

    RejectFilterHandler() :
        rejectCalls(0) {
    }

    bool filter(boost::shared_ptr<StubReceiver> &/*receiver*/, const int &/*message*/) {
        boost::mutex::scoped_lock lock(rejectMutex);
        ++rejectCalls;
        rejectCondition.notify_all();
        return false;
    }

};

TEST(OrderedQueueDispatcherPoolTest, testFilterExecution)
{

    StubReceiver::nextReceiverNum = 1;
    rejectCalls = 0;

    srand(time(NULL));

    OrderedQueueDispatcherPool<int, StubReceiver> pool(2,
            boost::bind(deliver, _1, _2), rejectFilter);

    boost::shared_ptr<StubReceiver> receiver(new StubReceiver);
    pool.registerReceiver(receiver);

    pool.start();

    const unsigned int numMessages = 10;
    for (unsigned int i = 0; i < numMessages; ++i) {
        pool.push(i);
    }

    // wait for filtering
    {
        boost::mutex::scoped_lock lock(rejectMutex);
        while (rejectCalls < numMessages) {
            rejectCondition.wait(lock);
        }
    }

    EXPECT_EQ((size_t) 0, receiver->messages.size());

}

TEST(OrderedQueueDispatcherPoolTest, testFilterExecutionObjectOrientedInterface)
{

    StubReceiver::nextReceiverNum = 1;

    boost::shared_ptr<RejectFilterHandler> filter(new RejectFilterHandler);

    srand(time(NULL));

    OrderedQueueDispatcherPool<int, StubReceiver> pool(
            2,
            OrderedQueueDispatcherPool<int, StubReceiver>::DeliveryHandlerPtr(
                    new DeliveryHandler), filter);

    boost::shared_ptr<StubReceiver> receiver(new StubReceiver);
    pool.registerReceiver(receiver);

    pool.start();

    const unsigned int numMessages = 10;
    for (unsigned int i = 0; i < numMessages; ++i) {
        pool.push(i);
    }

    // wait for filtering
    {
        boost::mutex::scoped_lock lock(filter->rejectMutex);
        while (filter->rejectCalls < numMessages) {
            filter->rejectCondition.wait(lock);
        }
    }

    EXPECT_EQ((size_t) 0, receiver->messages.size());

}

TEST(OrderedQueueDispatcherPoolTest, testUnregister)
{

    OrderedQueueDispatcherPool<int, StubReceiver> pool(2,
            boost::bind(deliver, _1, _2));

    pool.start();

    boost::shared_ptr<StubReceiver> receiver(new StubReceiver);
    pool.registerReceiver(receiver);
    EXPECT_TRUE(pool.unregisterReceiver(receiver));
    EXPECT_FALSE(pool.unregisterReceiver(receiver));

    pool.push(42);

    pool.stop();

    boost::this_thread::sleep(boost::posix_time::millisec(100));

    EXPECT_EQ((size_t) 0, receiver->messages.size());

}
