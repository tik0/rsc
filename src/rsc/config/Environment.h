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

#include <string>

#include <boost/filesystem/path.hpp>

#include <rsc/logging/Logger.h>

#include "ConfigSource.h"
#include "rsc/rscexports.h"

namespace rsc {
namespace config {

/**
 * Return the home directory of the user associated with the current
 * process.
 *
 * @return A @ref boost::filesystem::path object representing the home
 *         directory of the user.
 * @throw std::runtime_error if no home directory for the user is available
 *
 * @author jmoringe
 */
RSC_EXPORT boost::filesystem::path userHomeDirectory();

/**
 * Return the canonical directory for configuration files of the user
 * associated with the current process.
 *
 * @return A @ref boost::filesystem::path object representing the
 *         configuration directory of the user.
 * @throw std::runtime_error if no config directory for the user is available
 *
 * @author jmoringe
 */
RSC_EXPORT boost::filesystem::path userConfigDirectory();


/**
 * Objects of this class analyze the environment of the current
 * process, finding environment variables whose name starts with a
 * specified string. The prefix is stripped from matching names and
 * the variables are converted to options and passed to the
 * @ref OptionHandler.
 *
 * @author jmoringe
 */
class RSC_EXPORT EnvironmentVariableSource : public ConfigSource {
public:
    /**
     * Construct a source that collect environment variables whose
     * name starts with @a prefix.
     *
     * @param prefix A prefix string against which all environment
     *               variables are matched.
     */
    EnvironmentVariableSource(const std::string& prefix = "");

    void provideOptions(OptionHandler& handler);
private:
    rsc::logging::LoggerPtr logger;

    const std::string prefix;
};

}
}
