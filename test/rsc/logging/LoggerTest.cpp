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

TEST(LoggerTest, testLogMacros)
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

class StoringLogger: public Logger {
public:
    virtual Level getLevel() const {
        return Logger::LEVEL_ALL;
    }

    virtual void setLevel(const Level& /*level*/) {
    }

    virtual string getName() const {
        return "StoringLogger";
    }

    virtual void setName(const string& /*name*/) {
    }

    virtual void log(const Level& level, const string& msg) {
        logCalls.push_back(make_pair(level, msg));
    }

    vector<pair<Logger::Level, string> > logCalls;

};

TEST(LoggerTest, testAssertionMacros) {

    boost::shared_ptr<StoringLogger> logger(new StoringLogger);

    RSCTRACE_EXPECT(false, logger, "false1");
    RSCDEBUG_EXPECT(false, logger, "false2");
    RSCINFO_EXPECT(false, logger, "false3");
    RSCWARN_EXPECT(false, logger, "false4");
    RSCERROR_EXPECT(false, logger, "false5");
    RSCFATAL_EXPECT(false, logger, "false6");

    RSCTRACE_EXPECT(true, logger, "true");
    RSCDEBUG_EXPECT(true, logger, "true");
    RSCINFO_EXPECT(true, logger, "true");
    RSCWARN_EXPECT(true == true, logger, "true");
    RSCERROR_EXPECT(true, logger, "true");
    RSCFATAL_EXPECT(true ==
            true, logger, "true");

    ASSERT_EQ(6, logger->logCalls.size());
    EXPECT_EQ(Logger::LEVEL_TRACE, logger->logCalls[0].first);
    EXPECT_EQ("false1\nfailed condition: false", logger->logCalls[0].second);
    EXPECT_EQ(Logger::LEVEL_DEBUG, logger->logCalls[1].first);
    EXPECT_EQ("false2\nfailed condition: false", logger->logCalls[1].second);
    EXPECT_EQ(Logger::LEVEL_INFO, logger->logCalls[2].first);
    EXPECT_EQ("false3\nfailed condition: false", logger->logCalls[2].second);
    EXPECT_EQ(Logger::LEVEL_WARN, logger->logCalls[3].first);
    EXPECT_EQ("false4\nfailed condition: false", logger->logCalls[3].second);
    EXPECT_EQ(Logger::LEVEL_ERROR, logger->logCalls[4].first);
    EXPECT_EQ("false5\nfailed condition: false", logger->logCalls[4].second);
    EXPECT_EQ(Logger::LEVEL_FATAL, logger->logCalls[5].first);
    EXPECT_EQ("false6\nfailed condition: false", logger->logCalls[5].second);

}
