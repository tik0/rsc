/* ============================================================
 *
 * This file is a part of RSC project
 *
 * Copyright (C) 2010 by Johannes Wienke <jwienke at techfak dot uni-bielefeld dot de>
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

#include "LoggerFactory.h"

#include <vector>

#include <boost/bind.hpp>
#include <boost/format.hpp>
#include <boost/function.hpp>
#include <boost/algorithm/string.hpp>

#include "ConsoleLoggingSystem.h"
#include "LoggerProxy.h"
#include "../runtime/ContainerIO.h"

using namespace std;
using namespace rsc::misc;

namespace rsc {
namespace logging {

/**
 * A simple tree representation for loggers.
 *
 * We do not use any special library but create our own one because all publicly
 * available libs like BGL are too big for this purpose and BGL is often not
 * included in boost distributions.
 *
 * @author jwienke
 */
class LoggerFactory::LoggerTreeNode {
public:

    /**
     * A unique representation of a name. From the string representation this
     * is the name split at each '.' char. Empty vector means root logger.
     */
    typedef vector<string> NamePath;

    LoggerTreeNode(const string& name, LoggerProxyPtr loggerProxy) :
            name(name), loggerProxy(loggerProxy) {
    }

    string getName() const {
        return name;
    }

    LoggerProxyPtr getLoggerProxy() const {
        return loggerProxy;
    }

    /**
     * Adds a child if it does not exist so far.
     *
     * @param child child to add
     * @return @c true if the new child was added, @c false if it already exited.
     */
    bool addChild(LoggerTreeNodePtr child) {
        // we rely on the fact that map only inserts if not present
        return children.insert(make_pair(child->getName(), child)).second;
    }

    typedef boost::function<LoggerProxyPtr(const NamePath& name)> CreateFunction;

    /**
     *
     * @param path
     * @param
     * @return the deepest child in the path
     */
    LoggerTreeNodePtr addChildren(const NamePath& path,
            CreateFunction createFn) {

        if (path.size() == 0) {
            throw invalid_argument("Empty path given");
        }

        NamePath subPath = path;
        subPath.erase(subPath.begin());

        if (!children.count(path.front())) {
            children[path.front()] = LoggerTreeNodePtr(
                    new LoggerTreeNode(path.front(), createFn(path)));
        }
        if (subPath.size() > 1) {
            return children[path.front()]->addChildren(subPath, createFn);
        } else {
            return children[path.front()];
        }

    }

    /**
     * Visitor interface to operate on the tree.
     *
     * @author jwienke
     */
    class Visitor {
    public:
        virtual ~Visitor() {
        }
        virtual void visit(const NamePath& path,
                LoggerProxyPtr loggerProxy) = 0;
    };
    typedef boost::shared_ptr<Visitor> VisitorPtr;

    /**
     * Visits every sub-node excluding this node. Depth-first strategy is used.
     *
     * @param visitor visitor to use
     */
    void visit(VisitorPtr visitor, const NamePath& thisPath = NamePath()) {

        for (map<string, LoggerTreeNodePtr>::const_iterator it =
                children.begin(); it != children.end(); ++it) {

            NamePath childPath = thisPath;
            childPath.push_back(it->first);
            visitor->visit(childPath, it->second->getLoggerProxy());
            it->second->visit(visitor, childPath);

        }

    }

    bool hasChild(const string& name) const {
        return children.count(name) == 1;
    }

    bool hasChild(const NamePath& path) const {
        if (path.size() == 0) {
            throw invalid_argument("Empty name path given");
        }
        map<string, LoggerTreeNodePtr>::const_iterator childIt = children.find(
                path.front());
        if (childIt == children.end()) {
            return false;
        }
        NamePath subPath = path;
        subPath.erase(subPath.begin());
        return childIt->second->hasChild(subPath);
    }

    LoggerTreeNodePtr getChild(const string& name) const {
        map<string, LoggerTreeNodePtr>::const_iterator it = children.find(name);
        if (it == children.end()) {
            throw invalid_argument(
                    boost::str(
                            boost::format(
                                    "No direct child with name %1 exists.")
                                    % name));
        }
        return it->second;
    }

    LoggerTreeNodePtr getChild(const NamePath& path) const {
        if (path.size() == 0) {
            throw invalid_argument("Empty name path given");
        }
        map<string, LoggerTreeNodePtr>::const_iterator childIt = children.find(
                path.front());
        if (childIt == children.end()) {
            throw invalid_argument(
                    boost::str(
                            boost::format(
                                    "No direct child with name %1 exists.")
                                    % name));
        }
        NamePath subPath = path;
        subPath.erase(subPath.begin());
        return childIt->second->getChild(subPath);
    }

    void clearChildren() {
        children.clear();
    }

    static NamePath nameToPath(const string& name) {
        NamePath path;
        // TODO add consistency checks
        boost::algorithm::split(path, name, boost::algorithm::is_any_of("."));
        return path;
    }

    static string pathToName(const NamePath& path) {
        stringstream s;
        for (NamePath::const_iterator it = path.begin(); it != path.end();
                ++it) {
            s << *it;
            if (it != --path.end()) {
                s << ".";
            }
        }
        return s.str();
    }

private:

    /**
     * New name part of this node. Somehow mirrored from loggerProxy because we
     * do not want to rely on the custom logger implementation correctness.
     */
    string name;
    LoggerProxyPtr loggerProxy;

    map<string, LoggerTreeNodePtr> children;

};

const string LoggerFactory::DEFAULT_LOGGING_SYSTEM =
        ConsoleLoggingSystem::getLoggerName();

LoggerFactory::LoggerFactory() :
        currentLevel(Logger::LEVEL_WARN) {
    reselectLoggingSystem();
    loggerTree.reset(
            new LoggerTreeNode("", createLogger(LoggerTreeNode::NamePath())));
}

LoggerFactory::~LoggerFactory() {
}

class LoggerFactory::ReselectVisitor: public LoggerTreeNode::Visitor {
public:

    ReselectVisitor(boost::shared_ptr<LoggingSystem> newSystem) :
            newSystem(newSystem) {
    }

    void visit(const LoggerTreeNode::NamePath& path,
            LoggerProxyPtr loggerProxy) {
        loggerProxy->setLogger(
                newSystem->createLogger(LoggerTreeNode::pathToName(path)));
    }
private:
    boost::shared_ptr<LoggingSystem> newSystem;
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
        loggerTree->getLoggerProxy()->setLogger(
                loggingSystem->createLogger(""));
        loggerTree->visit(
                LoggerTreeNode::VisitorPtr(new ReselectVisitor(loggingSystem)));
    }

}

LoggerProxyPtr LoggerFactory::createLogger(
        const LoggerTreeNode::NamePath& path) {
    LoggerPtr logger(
            loggingSystem->createLogger(LoggerTreeNode::pathToName(path)));
    logger->setLevel(currentLevel);
    LoggerProxyPtr proxy(new LoggerProxy(logger));
    return proxy;
}

LoggerPtr LoggerFactory::getLogger(const string& name) {
    boost::recursive_mutex::scoped_lock lock(mutex);
    LoggerTreeNode::NamePath path = LoggerTreeNode::nameToPath(name);
    if (path.empty()) {
        return loggerTree->getLoggerProxy();
    }
    LoggerTreeNodePtr node = loggerTree->addChildren(path,
            boost::bind(&LoggerFactory::createLogger, this, _1));
    return node->getLoggerProxy();
}

class LoggerFactory::ReconfigurationVisitor: public LoggerTreeNode::Visitor {
public:

    ReconfigurationVisitor(const Logger::Level &newLevel) :
            newLevel(newLevel) {
    }

    void visit(const LoggerTreeNode::NamePath& /*path*/,
            LoggerProxyPtr loggerProxy) {
        loggerProxy->setLevel(newLevel);
    }
private:
    Logger::Level newLevel;
};

void LoggerFactory::reconfigure(const Logger::Level& level) {
    boost::recursive_mutex::scoped_lock lock(mutex);
    currentLevel = level;
    //loggerTree->getLoggerProxy()->setLevel(currentLevel);
    loggerTree->visit(
            LoggerTreeNode::VisitorPtr(new ReconfigurationVisitor(level)));
}

string LoggerFactory::getLoggingSystemName() {
    boost::recursive_mutex::scoped_lock lock(mutex);
    return loggingSystem->getRegistryKey();
}

void LoggerFactory::clearKnownLoggers() {
    boost::recursive_mutex::scoped_lock lock(mutex);
    loggerTree->clearChildren();
}

}
}
