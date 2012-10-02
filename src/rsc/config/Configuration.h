/* ============================================================
 *
 * This file is part of the RSC project
 *
 * Copyright (C) 2012 Jan Moringen <jmoringe@techfak.uni-bielefeld.de>
 *
 * This file may be licensed under the terms of the
 * GNU Lesser General Public License Version 3 (the ``LGPL''),
 * or (at your option) any later version.
 *
 * Software distributed under the License is distributed
 * on an ``AS IS'' basis, WITHOUT WARRANTY OF ANY KIND, either
 * express or implied. See the LGPL for the specific language
 * governing rights and limitations.
 *
 * You should have received a copy of the LGPL along with this
 * program. If not, go to http://www.gnu.org/licenses/lgpl.html
 * or write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * The development of this software was supported by:
 *   CoR-Lab, Research Institute for Cognition and Robotics
 *     Bielefeld University
 *
 * ============================================================ */

#include <string>

#include "OptionHandler.h"

#include <rsc/rscexports.h>

namespace rsc {
namespace config {

/**
 * Pass configuration options in from configuration files derived from
 * @a configFileName and environment variables with prefix @a
 * environmentVariablePrefix to @a handler.
 *
 * The following configuration configuration sources are considered
 * based on @a configFileName:
 *
 * -# Prepending a system-wide configuration directory (e.g. @c /etc/)
 *    to @a configFileName
 * -# Prepending a user-specific configuration directory (e.g. @c
 *    $HOME/.config) to @a configFileName
 * -# Prepending the current working directory (@c $(pwd)) to @a
 *    configFileName
 * -# Environment Variables
 *
 * See #ConfigFileSource for the configuration file format.
 *
 * @param handler Receiver of the configuration options.
 * @param configFileName A filename (without directory) from which
 *                       three configuration file names are derived.
 * @param environmentVariablePrefix A prefix string with which all
 *                                  processed environment variables
 *                                  have to start.
 *
 * @author jmoringe
 */
void RSC_EXPORT configure(OptionHandler& handler,
                          const std::string& configFileName,
                          const std::string& environmentVariablePrefix);


}
}
