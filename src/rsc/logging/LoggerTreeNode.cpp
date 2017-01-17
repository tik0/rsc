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

#include "LoggerTreeNode.h"

#include <boost/format.hpp>
#include <boost/algorithm/string.hpp>

#include <rsc/runtime/ContainerIO.h>

using namespace std;

namespace rsc {
namespace logging {

LoggerTreeNode::LoggerTreeNode(const string& name, LoggerTreeNodeWeakPtr parent) :
        name(name), parent(parent), bHasAssignedLevel(false) {
}

LoggerTreeNode::LoggerTreeNode(const string& name, LoggerProxyPtr loggerProxy,
        LoggerTreeNodeWeakPtr parent) :
        name(name), loggerProxy(loggerProxy), parent(parent), bHasAssignedLevel(false) {
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

LoggerTreeNodePtr LoggerTreeNode::addChildren(const NamePath& path) {

    if (path.empty()) {
        return shared_from_this();
    }

    const string &directChildName = path.front();
    if (!children.count(directChildName)) {
        children[directChildName] = LoggerTreeNodePtr(
                new LoggerTreeNode(directChildName, shared_from_this()));
    }

    if (path.size() > 1) {
        NamePath subPath = path;
        subPath.erase(subPath.begin());
        return children[directChildName]->addChildren(subPath);
    } else {
        return children[directChildName];
    }
}

LoggerTreeNode::Visitor::~Visitor() {
}

void LoggerTreeNode::visit(VisitorPtr visitor, bool bValidProxiesOnly, const NamePath& thisPath) {
    bool descend = true;
    if (!bValidProxiesOnly || this->getLoggerProxy())
        descend = visitor->visit(thisPath, shared_from_this());
    if (!descend) return;

    for (map<string, LoggerTreeNodePtr>::const_iterator it = children.begin();
            it != children.end(); ++it) {

        NamePath childPath = thisPath;
        childPath.push_back(it->first);
        it->second->visit(visitor, bValidProxiesOnly, childPath);
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

    // TODO add checks which can be detected easiest in the string representation

    NamePath path;
    string lowerName = name;
    boost::algorithm::to_lower(lowerName);
    boost::algorithm::split(path, lowerName, boost::algorithm::is_any_of("."));

    // Some consistency checks
    assert(path.size() > 0);

    // avoid reserved keywords at the end
    // TODO how to avoid string duplication with OptionBasedConfigurator?
    // The configurator would be the place to store the two string constants in
    // but I do not want to have a dependency from this class to the
    // configurator.
    if (path.back() == "level" || path.back() == "system") {
        throw invalid_argument(
                "Logger names must not end with system or level (case-insensitive), but I received: '"
                        + name + "'");
    }

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

Logger::Level LoggerTreeNode::getLevel() const
{
    if (hasAssignedLevel()) return assignedLevel;
    return getParent()->getLevel();
}

void LoggerTreeNode::setAssignedLevel(const Logger::Level& level) {
    assignedLevel = level;
    bHasAssignedLevel = true;
}

void LoggerTreeNode::unsetAssignedLevel()
{
    bHasAssignedLevel = false;
}

bool LoggerTreeNode::hasAssignedLevel() const {
    return bHasAssignedLevel;
}

}
}
