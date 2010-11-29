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

#ifndef WINDOWSSUBPROCESS_H_
#define WINDOWSSUBPROCESS_H_

#include <string>
#include <vector>

#include "Subprocess.h"

namespace rsc {
namespace subprocess {

/**
 * Subprocess implementation for windows.
 *
 * @author jwienke
 */
class WindowsSubprocess: public Subprocess {
public:
    WindowsSubprocess(const std::string &command,
            const std::vector<std::string> &args = std::vector<std::string>());
    virtual ~WindowsSubprocess();
};

}
}

#endif /* WINDOWSSUBPROCESS_H_ */
