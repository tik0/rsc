/* ============================================================
 *
 * This file is a part of RSC project
 *
 * Copyright (C) 2010 by Johannes Wienke <jwienke at techfak dot uni-bielefeld dot de>
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

#include <stdexcept>
#include <vector>

#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/thread.hpp>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "rsc/logging/Logger.h"
#include "rsc/misc/langutils.h"

#include "mocks.h"

using namespace std;
using namespace rsc;
using namespace rsc::logging;
using namespace testing;

TEST(LoggerFactoryTest, testLogMacros)
{

    string expect = "one 2 three";

    map<Logger::Level, unsigned int> levelCalls;
    levelCalls.insert(pair<Logger::Level, unsigned int> (Logger::LEVEL_ALL, 0));
    levelCalls.insert(pair<Logger::Level, unsigned int> (Logger::LEVEL_TRACE, 0));
    levelCalls.insert(pair<Logger::Level, unsigned int> (Logger::LEVEL_DEBUG, 1));
    levelCalls.insert(pair<Logger::Level, unsigned int> (Logger::LEVEL_INFO, 2));
    levelCalls.insert(pair<Logger::Level, unsigned int> (Logger::LEVEL_WARN, 3));
    levelCalls.insert(pair<Logger::Level, unsigned int> (Logger::LEVEL_ERROR, 4));
    levelCalls.insert(pair<Logger::Level, unsigned int> (Logger::LEVEL_FATAL, 5));
    levelCalls.insert(pair<Logger::Level, unsigned int> (Logger::LEVEL_OFF, 6));

    vector<Logger::Level> expectedLevels;
    expectedLevels.push_back(Logger::LEVEL_TRACE);
    expectedLevels.push_back(Logger::LEVEL_DEBUG);
    expectedLevels.push_back(Logger::LEVEL_INFO);
    expectedLevels.push_back(Logger::LEVEL_WARN);
    expectedLevels.push_back(Logger::LEVEL_ERROR);
    expectedLevels.push_back(Logger::LEVEL_FATAL);

    for (map<Logger::Level, unsigned int>::iterator it = levelCalls.begin(); it
            != levelCalls.end(); ++it) {

        StrictMock<MockLogger> loggerMock;

        EXPECT_CALL(loggerMock, getLevel()).Times(AnyNumber()).WillRepeatedly(
                Return(it->first));
        for (unsigned int i = it->second; i < expectedLevels.size(); ++i) {
            EXPECT_CALL(loggerMock, log(expectedLevels[i], expect));
        }

        LoggerPtr logger(&loggerMock, rsc::misc::NullDeleter());

        RSCTRACE(logger, "one " << 2 << " three");
        RSCDEBUG(logger, "one " << 2 << " three");
        RSCINFO(logger, "one " << 2 << " three");
        RSCWARN(logger, "one " << 2 << " three");
        RSCERROR(logger, "one " << 2 << " three");
        RSCFATAL(logger, "one " << 2 << " three");

    }

}
