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

#pragma once

#include <queue>

#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <boost/thread/condition.hpp>
#include <boost/thread/recursive_mutex.hpp>

#include "InterruptedException.h"

namespace rsc {
namespace threading {

/**
 * Indicates that a queue was empty while trying to pop an element from it.
 *
 * @author jwienke
 */
class QueueEmptyException: public std::runtime_error {
public:
    QueueEmptyException();
    explicit QueueEmptyException(const std::string &message);
};

/**
 * A queue with synchronized access and interruption support. On #push
 * operations only one waiting thread is woken up.
 *
 * @author jwienke
 * @tparam M message type handled in this queue
 */
template<class M>
class SynchronizedQueue: public boost::noncopyable {
public:

    typedef boost::function<void(const M &drop)> dropHandlerType;

private:

    volatile bool interrupted;
    std::queue<M> queue;
    boost::recursive_mutex mutex;
    boost::condition condition;
    unsigned int sizeLimit;
    dropHandlerType dropHandler;

public:

    /**
     * Creates a new queue.
     *
     * @param sizeLimit max allowed size of the queue. If the limit is reached
     *                  and another element is added to the queue, the oldest
     *                  element is removed. 0 means unlimited
     * @param
     */
    explicit SynchronizedQueue(const unsigned int &sizeLimit = 0,
            dropHandlerType dropHandler = 0) :
        interrupted(false), sizeLimit(sizeLimit), dropHandler(dropHandler) {
    }

    virtual ~SynchronizedQueue() {
        // TODO what to do on destruction if still someone in waiting?
        // Simply calling interrupt does not help because threads still access
        // the queue variables at waking up from the interruption while this
        // class is being destructed and the condition variable gets destructed
    }

    /**
     * Pushes a new element on the queue and wakes up one waiting client if
     * there is one.
     *
     * @param message element to push on the queue
     */
    void push(const M &message) {
        {
            boost::recursive_mutex::scoped_lock lock(mutex);
            if (sizeLimit > 0) {
                while (queue.size() > sizeLimit - 1) {
                    if (dropHandler) {
                        dropHandler(queue.front());
                    }
                    queue.pop();
                }
            }
            queue.push(message);
        }
        condition.notify_one();
    }

    /**
     * Returns the next element form the queue and wait until there is such an
     * element. The returned element is removed from the queue immediately.
     *
     * @return next element on the queue
     * @throw InterruptedException if #interrupt was called before a call to
     *                             this method or while waiting for an element
     */
    M pop() {

        boost::recursive_mutex::scoped_lock lock(mutex);

        while (!interrupted && queue.empty()) {
            condition.wait(lock);
        }

        if (interrupted) {
            throw InterruptedException("Queue was interrupted");
        }

        M message = queue.front();
        queue.pop();
        return message;

    }

    /**
     * Tries to pop an element from the queue but does not wait if there is no
     * element on the queue.
     *
     * @return element from the queue
     * @throw QueueEmptyException the queue was empty
     */
    M tryPop() {

        boost::recursive_mutex::scoped_lock lock(mutex);

        if (queue.empty()) {
            throw QueueEmptyException();
        }

        M message = queue.front();
        queue.pop();
        return message;

    }

    /**
     * Checks whether this queue is empty. Is not affected by the interruption
     * flag.
     *
     * @return @c true if empty, else @c false
     */
    bool empty() {
        boost::recursive_mutex::scoped_lock lock(mutex);
        return queue.empty();
    }

    /**
     * Interrupts the processing on this queue for every current call to pop
     * and any later call. All of them will return with an exception.
     */
    void interrupt() {
        {
            boost::recursive_mutex::scoped_lock lock(mutex);
            interrupted = true;
        }
        condition.notify_all();
    }

};

}
}

