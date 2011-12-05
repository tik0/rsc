/* ============================================================
 *
 * This file is a part of the RSC project.
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

#include <boost/shared_ptr.hpp>

#include "Logger.h"

namespace rsc {
namespace logging {

/**
 * A proxy to an instance of Logger, which provides the same interface but
 * allows to exchange the underlying logger at runtime.
 *
 * @author jwienke
 */
class LoggerProxy: public Logger {
public:

    /**
     * Constructor.
     * @param logger the initial logger to hide behind this proxy
     */
    LoggerProxy(LoggerPtr logger);
    virtual ~LoggerProxy();

    /**
     * @name logger interface
     */
    //@{

    virtual Logger::Level getLevel() const;
    virtual void setLevel(const Logger::Level& level);
    virtual std::string getName() const;
    virtual void setName(const std::string& name);
    virtual void log(const Logger::Level& level, const std::string& msg);

    //@}

    /**
     * Returns the hidden logger behind this proxy.
     * @return hidden logger instance
     */
    LoggerPtr getLogger() const;

    /**
     * (Re-)Sets the logger to be hidden behind this proxy. This may happen at
     * any time and from any thread.
     *
     * @param logger new logger to hide
     */
    void setLogger(LoggerPtr logger);

private:

    /**
     * The hidden logger. We rely on the locking of boost::shared_ptr for access
     * to this instance, even on reselection.
     */
    LoggerPtr logger;

};

typedef boost::shared_ptr<LoggerProxy> LoggerProxyPtr;

}
}
