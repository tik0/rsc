/* ============================================================
 *
 * This file is a part of the RSC project
 *
 * Copyright (C) 2010 by Sebastian Wrede <swrede at techfak dot uni-bielefeld dot de>
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

#include <boost/shared_ptr.hpp>

#include "../logging/Logger.h"
#include "rsc/rscexports.h"

namespace rsc {
namespace threading {

/**
 * A task is a self.contained piece of work that can be executed by another
 * thread. Tasks can be canceled and interested clients can wait until a task
 * has finished its work.
 *
 * @author swrede
 * @author jwienke
 */
class RSC_EXPORT Task {
public:

    /**
     * Interrupts the task as soon as possible.
     */
    virtual void cancel() = 0;

    /**
     * Tells whether the task was interrupted by a call to #cancel.n.
     *
     * @return @c true if #cancel was called, else @c false
     */
    virtual bool isCancelRequested() = 0;

    /**
     * Performs the real task work. Should check for canceling as often as
     * possible.
     */
    virtual void run() = 0;

    /**
     * Waits for the execution of this task to finish. This method must block
     * until #run exits.
     */
    virtual void waitDone() = 0;

};

typedef boost::shared_ptr<Task> TaskPtr;

}
}

