/* ============================================================
 *
 * This file is a part of RSC project
 *
 * Copyright (C) 2010 by Johannes Wienke <jwienke at techfak dot uni-bielefeld dot de>
 *
 * This file may be licensed under the terms of of the
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

#include <stdexcept>
#include <vector>

#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/thread.hpp>

#include <gtest/gtest.h>

#include "rsc/logging/ConsoleLogger.h"

using namespace std;
using namespace rsc;
using namespace rsc::logging;

TEST(ConsoleLoggerTest, testLog)
{

    vector<Logger::Level> loggerLevels;
    loggerLevels.push_back(Logger::LEVEL_ALL);
    loggerLevels.push_back(Logger::LEVEL_TRACE);
    loggerLevels.push_back(Logger::LEVEL_DEBUG);
    loggerLevels.push_back(Logger::LEVEL_INFO);
    loggerLevels.push_back(Logger::LEVEL_WARN);
    loggerLevels.push_back(Logger::LEVEL_ERROR);
    loggerLevels.push_back(Logger::LEVEL_FATAL);
    loggerLevels.push_back(Logger::LEVEL_OFF);

    vector<Logger::Level> testLevels;
    testLevels.push_back(Logger::LEVEL_TRACE);
    testLevels.push_back(Logger::LEVEL_DEBUG);
    testLevels.push_back(Logger::LEVEL_INFO);
    testLevels.push_back(Logger::LEVEL_WARN);
    testLevels.push_back(Logger::LEVEL_ERROR);
    testLevels.push_back(Logger::LEVEL_FATAL);

    for (vector<Logger::Level>::iterator loggerLevelIt = loggerLevels.begin(); loggerLevelIt
            != loggerLevels.end(); ++loggerLevelIt) {

        Logger::Level loggerLevel = *loggerLevelIt;

        cout << "LoggerLevel: " << loggerLevel << endl;
        ConsoleLogger logger("TestLogger", loggerLevel);

        for (vector<Logger::Level>::iterator testLevelIt = testLevels.begin(); testLevelIt
                    != testLevels.end(); ++testLevelIt) {
            stringstream msg;
            msg << "Testing level " << *testLevelIt;
            logger.log(*testLevelIt, msg.str());

            if (*testLevelIt <= loggerLevel) {
                EXPECT_TRUE(logger.isEnabledFor(*testLevelIt));
            } else {
                EXPECT_FALSE(logger.isEnabledFor(*testLevelIt));
            }

        }

    }

}
