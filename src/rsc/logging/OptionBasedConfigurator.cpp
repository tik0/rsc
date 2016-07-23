/* ============================================================
 *
 * This file is a part of the RSC project.
 *
 * Copyright (C) 2012 by Johannes Wienke <jwienke at techfak dot uni-bielefeld dot de>
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

#include "OptionBasedConfigurator.h"

#include <boost/algorithm/string.hpp>

#include "Logger.h"
#include "LoggerFactory.h"
#include "../runtime/ContainerIO.h"

using namespace std;
using namespace rsc::config;

namespace rsc {
namespace logging {

OptionBasedConfigurator::OptionBasedConfigurator(
        const vector<string>& rootOption) :
        rootOption(normalizeKey(rootOption)) {
}

OptionBasedConfigurator::~OptionBasedConfigurator() {
}

vector<string> OptionBasedConfigurator::getDefaultRootOption() {
    vector<string> root;
    root.push_back("rsc");
    root.push_back("logging");
    return root;
}

vector<string> OptionBasedConfigurator::getRootOption() const {
    return rootOption;
}

bool OptionBasedConfigurator::keyStartWithRoot(
        const vector<string>& key) const {

    if (key.size() < rootOption.size()) {
        return false;
    }

    for (size_t i = 0; i < rootOption.size(); ++i) {
        string keyPart = key[i];
        boost::algorithm::to_lower(keyPart);
        if (rootOption[i] != keyPart) {
            return false;
        }
    }

    return true;

}

string OptionBasedConfigurator::loggerNameFromKey(
        const vector<string>& key) const {
    string name;
    // the last fragment must be the setting, do not include in the name
    for (vector<string>::const_iterator keyIt = key.begin() + rootOption.size();
            keyIt != key.end() - 1; ++keyIt) {
        name += "." + *keyIt;
    }

    if (name.empty()) {
        return name;
    } else {
        return name.substr(1, name.size() - 1);
    }

}

void OptionBasedConfigurator::handleOption(const vector<string>& key,
        const string& value) {

    vector<string> normalizedKey = normalizeKey(key);

    if (!keyStartWithRoot(normalizedKey)) {
        return;
    }

    // something can only be a valid key for the configuration if there is a
    // chance that a special configuration key is at the end. Hence, sufficient
    // length is required
    if (normalizedKey.size() <= rootOption.size()) {
        return;
    }

    LoggerPtr logger = Logger::getLogger(loggerNameFromKey(normalizedKey));

    string setting = normalizedKey.back();
    if (setting == "level") {

        if (value == "ALL") {
            logger->setLevel(Logger::LEVEL_ALL);
        } else if (value == "TRACE") {
            logger->setLevel(Logger::LEVEL_TRACE);
        } else if (value == "DEBUG") {
            logger->setLevel(Logger::LEVEL_DEBUG);
        } else if (value == "INFO") {
            logger->setLevel(Logger::LEVEL_INFO);
        } else if (value == "WARN") {
            logger->setLevel(Logger::LEVEL_WARN);
        } else if (value == "ERROR") {
            logger->setLevel(Logger::LEVEL_ERROR);
        } else if (value == "FATAL") {
            logger->setLevel(Logger::LEVEL_FATAL);
        } else if (value == "OFF") {
            logger->setLevel(Logger::LEVEL_OFF);
        }

    } else if (setting == "system" && logger->getName() == "") {
        LoggerFactory::getInstance().reselectLoggingSystem(value);
    }

}

vector<string> OptionBasedConfigurator::normalizeKey(
        const vector<string>& key) const {

    vector<string> normalizedKey;
    for (vector<string>::const_iterator keyIt = key.begin(); keyIt != key.end(); ++keyIt) {
        string part = *keyIt;
        boost::algorithm::to_lower(part);
        normalizedKey.push_back(part);
    }

    return normalizedKey;

}

}
}
