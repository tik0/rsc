/* ============================================================
 *
 * This file is part of the RSC project
 *
 * Copyright (C) 2011 Jan Moringen
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

#include <boost/filesystem/path.hpp>

#include <rsc/logging/Logger.h>

#include "ConfigSource.h"
#include "rsc/rscexports.h"

namespace rsc {
namespace config {

/** Return the home directory of the user associated with the current
 * process.
 *
 * @return A @ref boost::filesystem::path object representing the home
 * directory of the user.
 *
 * @author jmoringe
 */
RSC_EXPORT boost::filesystem::path userHomeDirectory();

/** Return the canonical directory for configuration files of the user
 * associated with the current process.
 *
 * @return A @ref boost::filesystem::path object representing the
 * configuration directory of the user.
 *
 * @author jmoringe
 */
RSC_EXPORT boost::filesystem::path userConfigDirectory();


/** Objects of this class analyze the environment of the current
 * process, finding environment variables which are named RSB_*. These
 * are converted to options and passed to the @ref OptionHandler.
 *
 * @author jmoringe
 */
RSC_EXPORT class EnvironmentVariableSource : public ConfigSource {
public:
    EnvironmentVariableSource();

    void emit(OptionHandler &handler);
private:
    rsc::logging::LoggerPtr logger;
};

}
}
