/* ============================================================
 *
 * This file is a part of the RSC project.
 *
 * Copyright (C) 2011 by Johannes Wienke <jwienke at techfak dot uni-bielefeld dot de>
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

#include "LoggerTreeNode.h"

#include <boost/format.hpp>
#include <boost/algorithm/string.hpp>

#include <rsc/runtime/ContainerIO.h>

using namespace std;

namespace rsc {
namespace logging {

LoggerTreeNode::LoggerTreeNode(const string& name, LoggerTreeNodeWeakPtr parent) :
        name(name), parent(parent) {
}

LoggerTreeNode::LoggerTreeNode(const string& name, LoggerProxyPtr loggerProxy,
        LoggerTreeNodeWeakPtr parent) :
        name(name), loggerProxy(loggerProxy), parent(parent) {
}

LoggerTreeNodePtr LoggerTreeNode::getParent() const {
    return parent.lock();
}

string LoggerTreeNode::getName() const {
    return name;
}

LoggerProxyPtr LoggerTreeNode::getLoggerProxy() const {
    return loggerProxy;
}

void LoggerTreeNode::setLoggerProxy(LoggerProxyPtr loggerProxy) {
    this->loggerProxy = loggerProxy;
}

bool LoggerTreeNode::addChild(LoggerTreeNodePtr child) {
    // we rely on the fact that map only inserts if not present
    return children.insert(make_pair(child->getName(), child)).second;
}

LoggerTreeNodePtr LoggerTreeNode::addChildren(const NamePath& path,
        CreateFunction createFn, const NamePath &processedPath) {

    if (path.size() == 0) {
        throw invalid_argument("Empty path given");
    }

    const string directChildName = path.front();
    NamePath subPath = path;
    subPath.erase(subPath.begin());
    NamePath childPath = processedPath;
    childPath.push_back(directChildName);

    if (!children.count(path.front())) {
        children[path.front()] = LoggerTreeNodePtr(
                new LoggerTreeNode(path.front(), shared_from_this()));
        children[path.front()]->setLoggerProxy(
                createFn(childPath, children[directChildName]));
    }

    if (path.size() > 1) {
        return children[directChildName]->addChildren(subPath, createFn,
                childPath);
    } else {
        return children[directChildName];
    }

}

LoggerTreeNode::Visitor::~Visitor() {
}

void LoggerTreeNode::visit(VisitorPtr visitor, const NamePath& thisPath) {

    for (map<string, LoggerTreeNodePtr>::const_iterator it = children.begin();
            it != children.end(); ++it) {

        NamePath childPath = thisPath;
        childPath.push_back(it->first);

        bool descend = visitor->visit(childPath, it->second,
                loggerProxy->getLogger()->getLevel());
        if (descend) {
            it->second->visit(visitor, childPath);
        }

    }

}

bool LoggerTreeNode::hasChild(const string& name) const {
    return children.count(name) == 1;
}

bool LoggerTreeNode::hasChild(const NamePath& path) const {
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

LoggerTreeNodePtr LoggerTreeNode::getChild(const string& name) const {
    map<string, LoggerTreeNodePtr>::const_iterator it = children.find(name);
    if (it == children.end()) {
        throw invalid_argument(
                boost::str(
                        boost::format("No direct child with name %1 exists.")
                                % name));
    }
    return it->second;
}

LoggerTreeNodePtr LoggerTreeNode::getChild(const NamePath& path) const {
    if (path.size() == 0) {
        throw invalid_argument("Empty name path given");
    }
    map<string, LoggerTreeNodePtr>::const_iterator childIt = children.find(
            path.front());
    if (childIt == children.end()) {
        throw invalid_argument(
                boost::str(
                        boost::format("No direct child with name %1 exists.")
                                % name));
    }
    NamePath subPath = path;
    subPath.erase(subPath.begin());
    return childIt->second->getChild(subPath);
}

void LoggerTreeNode::clearChildren() {
    children.clear();
}

LoggerTreeNode::NamePath LoggerTreeNode::nameToPath(const string& name) {
    if (name.empty()) {
        return NamePath();
    }
    NamePath path;
    // TODO add consistency checks
    boost::algorithm::split(path, name, boost::algorithm::is_any_of("."));
    return path;
}

string LoggerTreeNode::pathToName(const NamePath& path) {
    stringstream s;
    for (NamePath::const_iterator it = path.begin(); it != path.end(); ++it) {
        s << *it;
        if (it != --path.end()) {
            s << ".";
        }
    }
    return s.str();
}

boost::shared_ptr<Logger::Level> LoggerTreeNode::getAssignedLevel() const {
    return assignedLevel;
}

void LoggerTreeNode::setAssignedLevel(boost::shared_ptr<Logger::Level> level) {
    this->assignedLevel = level;
}

void LoggerTreeNode::setAssignedLevel(const Logger::Level& level) {
    assignedLevel.reset(new Logger::Level(level));
}

bool LoggerTreeNode::hasAssignedLevel() const {
    return assignedLevel;
}

}
}
