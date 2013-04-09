/* ============================================================
 *
 * This file is a part of the rsc project.
 *
 * Copyright (C) 2013 by Johannes Wienke <jwienke at techfak dot uni-bielefeld dot de>
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

#include "CommandLinePropertySource.h"

#include <stdexcept>
#include <vector>

using namespace std;

namespace rsc {
namespace config {

CommandLinePropertySource::CommandLinePropertySource(int          argc,
                                                     const char** argv,
                                                     bool         reportSyntaxErrors,
                                                     char         option) :
    logger(logging::Logger::getLogger("rsc.config.CommandLineSource")),
    argc(argc), argv(argv),
    reportSyntaxErrors(reportSyntaxErrors), option(option) {
}

CommandLinePropertySource::~CommandLinePropertySource() {
}

void CommandLinePropertySource::provideOptions(OptionHandler& handler) {

    for (int i = 1; i < argc; ++i) {

        string rawOption = argv[i];
        RSCTRACE(logger, "Processing raw option '" << rawOption << "'");

        // there must be enough room for -D
        if (rawOption.size() < 2) {
            RSCTRACE(logger, "Ignoring uninteresting option '" << rawOption << "'");
            continue;
        }

        // check whether the separator -- for positional arguments was reached
        // if so, stop all processing
        if (rawOption == "--") {
            break;
        }

        // check whether we want to process this option at all
        if (rawOption.substr(0, 2) != string("-") + option) {
            RSCTRACE(logger, "Ignoring uninteresting option '" << rawOption << "'");
            continue;
        }

        // check whether we need to go to the next argument because only the flag
        // was given
        if (rawOption.length() == 2 && i < argc - 1) {
            ++i;
            rawOption = argv[i];
        } else {
            rawOption = rawOption.substr(2, rawOption.length() - 2);
        }

        size_t equalsPos = rawOption.find_first_of('=');
        if (equalsPos == string::npos) {
            RSCWARN(logger,
                    "Cannot parse option '" << rawOption << "'. Reason: = character missing");
            if (reportSyntaxErrors) {
                throw invalid_argument(
                        "Cannot parse option '" + rawOption
                                + "'. Reason: = character missing");
            } else {
                continue;
            }
        }

        string rawKey = rawOption.substr(0, equalsPos);
        string value = rawOption.substr(equalsPos + 1,
                rawOption.length() - equalsPos - 1);

        RSCTRACE(logger,
                "Parsed rawKey = '" << rawKey << "', value = '" << value << "'");

        vector<string> key;
        // preserve original backtrace
        if (reportSyntaxErrors) {
            splitKeyAtDots(rawKey, key);
        } else {
            try {
                splitKeyAtDots(rawKey, key);
            } catch (invalid_argument& e) {
                RSCWARN(logger, "Unable to parse option key: " << e.what());
                continue;
            }
        }

        handler.handleOption(key, value);

    }

}

}
}
