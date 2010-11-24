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

#ifndef CONSOLELOGGER_H_
#define CONSOLELOGGER_H_

#include <boost/thread/recursive_mutex.hpp>

#include "Logger.h"
#include "rsc/rscexports.h"

namespace rsc {
namespace logging {

/**
 * A simple logger that uses cout and cerr for logging.
 *
 * @author jwienke
 */
class RSC_EXPORT ConsoleLogger: public Logger {
public:

    /**
     * Creates a new logger with the given name and level INFO.
     *
     * @param name name of the logger
     */
    ConsoleLogger(const std::string &name);
    ConsoleLogger(const std::string &name, const Level &level);
    virtual ~ConsoleLogger();

    Level getLevel() const;
    void setLevel(const Level &level);
    std::string getName() const;
    void setName(const std::string &name);

    void log(const Level &level, const std::string &msg);

private:

    /**
     * Prints a generic header for this logger to the stream. Acquire the lock
     * before calling this method.
     *
     * @param stream stream to print on
     * @param level the level of the header to generate
     * @return the stream passed in
     */
    std::ostream &printHeader(std::ostream &stream, const Level &level);

    std::string name;
    Level level;

    mutable boost::recursive_mutex mutex;

};

}
}

#endif /* CONSOLELOGGER_H_ */
