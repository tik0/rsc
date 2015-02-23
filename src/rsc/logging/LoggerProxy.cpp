/* ============================================================
 *
 * This file is a part of the RSC project.
 *
 * Copyright (C) 2011 by Johannes Wienke <jwienke at techfak dot uni-bielefeld dot de>
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

#include "LoggerProxy.h"

namespace rsc {
namespace logging {

LoggerProxy::SetLevelCallback::~SetLevelCallback() {
}

LoggerProxy::LoggerProxy(LoggerPtr logger, SetLevelCallbackPtr callback) :
        logger(logger), callback(callback) {
}

LoggerProxy::~LoggerProxy() {
}

Logger::Level LoggerProxy::getLevel() const {
    return logger->getLevel();
}

void LoggerProxy::setLevel(const Logger::Level& level) {
    callback->call(shared_from_this(), level);
}

std::string LoggerProxy::getName() const {
    return logger->getName();
}

void LoggerProxy::setName(const std::string& name) {
    logger->setName(name);
}

void LoggerProxy::log(const Logger::Level& level, const std::string& msg) {
    logger->log(level, msg);
}

LoggerPtr LoggerProxy::getLogger() const {
    return logger;
}

void LoggerProxy::setLogger(LoggerPtr logger) {
    this->logger = logger;
}

void LoggerProxy::setLevelCallback(const LoggerProxy::SetLevelCallbackPtr &setter)
{
    this->callback = setter;
}

}
}
