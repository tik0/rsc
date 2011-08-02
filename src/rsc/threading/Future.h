/* ============================================================
 *
 * This file is a part of the the RSC project.
 *
 * Copyright (C) 2011 by Johannes Wienke <jwienke at techfak dot uni-bielefeld dot de>
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

#include <stdexcept>

#include <boost/thread/condition.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/shared_ptr.hpp>

#include "rsc/rscexports.h"

namespace rsc {
namespace threading {

/**
 * Exception that is thrown if the result of a Future is not available because
 * the underlying process generated an error.
 *
 * @author jwienke
 */
class RSC_EXPORT FutureTaskExecutionException: public std::runtime_error {
public:

    /**
     * Constructor.
     *
     * @param msg error message
     */
    explicit FutureTaskExecutionException(const std::string &msg);

};

/**
 * Class providing access to the result of a process that is asynchronously
 * running. If the result is requested before the task finished, the ::get
 * operation will block until a results or error is available.
 *
 * @author jwienke
 * @tparam R result type, should be copyable but this is not a hard requirement
 */
template<class R>
class Future {

private:

    R result;
    bool taskFinished;
    bool taskError;
    std::string errorMsg;
    boost::condition condition;
    boost::mutex mutex;

public:

    /**
     * Constructor.
     */
    Future() :
        taskFinished(false), taskError(false) {
    }

    /**
     * Waits if necessary for the computation to complete, and then retrieves
     * its result.
     *
     * @return the computation result
     * @throw FutureTaskExecutionException the computation providing the result
     *                                     of this Future returned with an error
     */
    R get() {

        boost::mutex::scoped_lock lock(mutex);
        while (!taskFinished) {
            condition.wait(lock);
        }

        if (taskError) {
            throw FutureTaskExecutionException(errorMsg);
        }

        return result;

    }

    /**
     * Tells whether the computation of the underlying process finished and
     * provided a result or generated an error.
     *
     * @return @c true if a result or error is available, else @c false
     */
    bool isDone() {
        boost::mutex::scoped_lock lock(mutex);
        return taskFinished;
    }

    /**
     * Provide the result for this future.
     *
     * @param data result data
     */
    void set(R data) {
        result = data;
        {
            boost::mutex::scoped_lock lock(mutex);
            taskFinished = true;
            condition.notify_all();
        }
    }

    /**
     * Indicate an error while processing.
     *
     * @param message error description
     */
    void setError(const std::string &message) {
        errorMsg = message;
        {
            boost::mutex::scoped_lock lock(mutex);
            taskFinished = true;
            taskError = true;
        }
        condition.notify_all();
    }

};

}
}

