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

#ifndef SUBPROCESS_H_
#define SUBPROCESS_H_

#include <string>
#include <vector>

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

namespace rsc {
namespace subprocess {

class Subprocess;
typedef boost::shared_ptr<Subprocess> SubprocessPtr;

/**
 * A wrapper to call a different command as a subprocess and control its
 * lifecycle. This class uses the RAII idiom to manage the subprocces. This
 * means that the called process will be terminated on destruction of an
 * instance of this class.
 *
 * This class is intended to provide a platform-independent way of controlling
 * a command. Hence, a factory method is used instead of a normal constructor
 * to automatically instantiate the platform-specific implementations.
 *
 * Subclasses ensure that the process is started in the constructor and
 * terminated in the destructor.
 *
 * @author jwienke
 */
class Subprocess: public boost::noncopyable {
public:
    virtual ~Subprocess();

    /**
     * Creates a new subprocess for the given command with the specified
     * arguments.
     *
     * @param command command to call
     * @param args arguments for the command. The command itself must not be
     *             given. It is automatically passed by this class.
     * @return subprocess instance
     * @throw std::runtime_error error starting the command
     */
    static SubprocessPtr newInstance(const std::string &command,
            const std::vector<std::string> args = std::vector<std::string>());

protected:
    Subprocess();

};

}
}

#endif /* SUBPROCESS_H_ */
