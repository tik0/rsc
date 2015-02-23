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

#include "LoggerFactory.h"

#include <vector>

#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/format.hpp>
#include <boost/function.hpp>
#include <boost/algorithm/string.hpp>

#include "ConsoleLoggingSystem.h"
#include "LoggerProxy.h"
#include "OptionBasedConfigurator.h"
#include "../config/ConfigFileSource.h"
#include "../runtime/ContainerIO.h"

using namespace std;
using namespace rsc::misc;

namespace rsc {
namespace logging {

const string LoggerFactory::DEFAULT_LOGGING_SYSTEM =
        ConsoleLoggingSystem::getName();
const Logger::Level LoggerFactory::DEFAULT_LEVEL = Logger::LEVEL_WARN;

/**
 * @author rhaschke
 */
class SimpleLevelUpdater : public LoggerProxy::SetLevelCallback {
public:
    SimpleLevelUpdater(LoggerTreeNodeWeakPtr treeNode) :
            treeNode(treeNode) {
    }

    virtual ~SimpleLevelUpdater() {
    }

    void call(LoggerProxyPtr proxy, const Logger::Level& level) {
        treeNode.lock()->setAssignedLevel(level);
        proxy->getLogger()->setLevel(level);
    }

protected:
    LoggerTreeNodeWeakPtr treeNode;
};

/**
 * @author jwienke
 */
class TreeLevelUpdater: public SimpleLevelUpdater {
public:

    /**
     * A Visitor that propagates a logging level down the logger tree but stops
     * if a logger already as a level assigned.
     *
     * @author jwienke
     */
    class LevelSetter: public LoggerTreeNode::Visitor {
    public:

        LevelSetter(const Logger::Level& level) :
                level(level) {
        }

        virtual ~LevelSetter() {
        }

        bool visit(const LoggerTreeNode::NamePath& /*path*/,
                   LoggerTreeNodePtr node) {

            if (node->hasAssignedLevel()) {
                return false;
            }

            node->getLoggerProxy()->getLogger()->setLevel(level);
            return true;
        }

    private:
        Logger::Level level;
    };

    TreeLevelUpdater(LoggerTreeNodeWeakPtr treeNode,
            boost::recursive_mutex& mutex) :
            SimpleLevelUpdater(treeNode), mutex(mutex) {
    }

    virtual ~TreeLevelUpdater() {
    }

    // TODO jwienke: maybe it would be better to make this signature contain the node instead of the proxy? Can this work?
    void call(LoggerProxyPtr proxy, const Logger::Level& level) {
        LoggerTreeNodePtr node = treeNode.lock();
        // unset assigned level, so that recursive LevelSetter proceeds with node
        node->unsetAssignedLevel();
        boost::recursive_mutex::scoped_lock lock(mutex);
        node->visit(LoggerTreeNode::VisitorPtr(new LevelSetter(level)));
        // now set the level explicitly (sets assigned level)
        SimpleLevelUpdater::call(proxy, level);
    }

private:
    boost::recursive_mutex& mutex;
};


LoggerFactory::LoggerFactory() {
    reselectLoggingSystem();
    loggerTree.reset(new LoggerTreeNode("", LoggerTreeNodePtr()));
    loggerTree->setAssignedLevel(DEFAULT_LEVEL);
}

LoggerFactory::~LoggerFactory() {
}

class LoggerFactory::ReselectVisitor: public LoggerTreeNode::Visitor {
public:

    ReselectVisitor(LoggerFactory &factory) :
            factory(factory) {
    }

    bool visit(const LoggerTreeNode::NamePath& path, LoggerTreeNodePtr node) {
        // recreate proxy because we might need a different SetLevelCallback
        node->setLoggerProxy(factory.createLoggerProxy(
                                 LoggerTreeNode::pathToName(path), node));
        return true;
    }
private:
    LoggerFactory &factory;
};

void LoggerFactory::reselectLoggingSystem(const std::string& nameHint) {

    set<string> keys = loggingSystemRegistry()->getKnownRegistryKeys();
    assert(keys.count(DEFAULT_LOGGING_SYSTEM));

    string systemName;
    if (nameHint == DEFAULT_LOGGING_SYSTEM) {
        // use default if explicitly requested

        systemName = DEFAULT_LOGGING_SYSTEM;

    } else if (!nameHint.empty() && keys.count(nameHint)) {
        // use hint if available

        systemName = nameHint;

    } else {
        // auto-select fallback

        if (keys.size() > 1) {
            // do not use default if other options are available
            keys.erase(DEFAULT_LOGGING_SYSTEM);
        }
        systemName = *(keys.begin());

    }

    boost::recursive_mutex::scoped_lock lock(mutex);

    loggingSystem = loggingSystemRegistry()->getRegistree(systemName);

    // update existing loggers to use the new logging system
    if (loggerTree) {
        loggerTree->visit(
                LoggerTreeNode::VisitorPtr(new ReselectVisitor(*this)));
    }

}

LoggerProxyPtr LoggerFactory::createLoggerProxy(const std::string &name,
        LoggerTreeNodePtr node) {
    LoggerPtr logger(loggingSystem->createLogger(name,
                                                 node->getLevel()));
    LoggerProxy::SetLevelCallbackPtr callback;
    if (loggingSystem->needsRecursiveLevelSetting())
        callback.reset(new TreeLevelUpdater(LoggerTreeNodeWeakPtr(node), mutex));
    else
        callback.reset(new SimpleLevelUpdater(LoggerTreeNodeWeakPtr(node)));
    LoggerProxyPtr proxy(new LoggerProxy(logger, callback));
    return proxy;
}

LoggerPtr LoggerFactory::getLogger(const string& name) {
    boost::recursive_mutex::scoped_lock lock(mutex);
    LoggerTreeNode::NamePath path = LoggerTreeNode::nameToPath(name);
    LoggerTreeNodePtr node = loggerTree->addChildren(path);
    if (!node->getLoggerProxy()) {
        node->setLoggerProxy(this->createLoggerProxy(
                                 LoggerTreeNode::pathToName(path), node));
    }
    return node->getLoggerProxy();
}

class LoggerFactory::ReconfigurationVisitor: public LoggerTreeNode::Visitor {
public:

    ReconfigurationVisitor(const Logger::Level &newLevel, bool bRecursive) :
            newLevel(newLevel), bRecursive(bRecursive) {
    }

    bool visit(const LoggerTreeNode::NamePath& /*path*/, LoggerTreeNodePtr node) {
        if (node->hasAssignedLevel()) {
            node->setAssignedLevel(newLevel);
        }
        LoggerProxyPtr proxy = node->getLoggerProxy();
        if (bRecursive || node->hasAssignedLevel()) {
            proxy->getLogger()->setLevel(newLevel);
        }
        return true;

    }
private:
    Logger::Level newLevel;
    bool bRecursive;
};

void LoggerFactory::reconfigure(const Logger::Level& level) {
    boost::recursive_mutex::scoped_lock lock(mutex);
    loggerTree->setAssignedLevel(level);
    loggerTree->visit(
            LoggerTreeNode::VisitorPtr(
                    new ReconfigurationVisitor(level,
                                               loggingSystem->needsRecursiveLevelSetting())));
}

string LoggerFactory::getLoggingSystemName() {
    boost::recursive_mutex::scoped_lock lock(mutex);
    return loggingSystem->getRegistryKey();
}

void LoggerFactory::clearKnownLoggers() {
    boost::recursive_mutex::scoped_lock lock(mutex);
    loggerTree->clearChildren();
}

void LoggerFactory::reconfigureFromFile(const string& fileName) {
    OptionBasedConfigurator configurator;
    boost::filesystem::ifstream stream(fileName);
    if (!stream) {
        throw invalid_argument("Unable to open file " + fileName);
    }
    config::ConfigFileSource(stream).provideOptions(configurator);
}

}
}
