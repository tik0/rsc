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

#include <vector>

#include <boost/program_options.hpp>

using namespace std;
namespace po = boost::program_options;

namespace rsc {
namespace config {

CommandLinePropertySource::CommandLinePropertySource(const int& argc,
        char** argv, const bool& reportSyntaxErrors, const char& option) :
        logger(logging::Logger::getLogger("rsc.config.CommandLineSource")), argc(
                argc), argv(argv), reportSyntaxErrors(reportSyntaxErrors), option(
                option) {
}

CommandLinePropertySource::~CommandLinePropertySource() {
}

void CommandLinePropertySource::provideOptions(OptionHandler& handler) {

    vector<string> rawOptions;

    po::options_description desc("Allowed options");
    desc.add_options()((string(",") + option).c_str(),
            po::value<vector<string> >(&rawOptions),
            "Define an option for the RSC configuration mechanism");

    po::variables_map vm;
    po::store(
            po::command_line_parser(argc, argv).options(desc).allow_unregistered().run(),
            vm);
    po::notify(vm);

    for (vector<string>::const_iterator rawOptionIt = rawOptions.begin();
            rawOptionIt != rawOptions.end(); ++rawOptionIt) {

        string rawOption = *rawOptionIt;
        RSCTRACE(logger, "Processing raw option '" << rawOption << "'");

        size_t equalsPos = rawOption.find_first_of('=');
        if (equalsPos == string::npos) {
            RSCWARN(logger,
                    "Cannot parse option '" << rawOption << "'. Reason: = character missing");
            continue;
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
