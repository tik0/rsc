/* ============================================================
 *
 * This file is a part of the RSC project.
 *
 * Copyright (C) 2011 by Johannes Wienke <jwienke at techfak dot uni-bielefeld dot de>
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

#pragma once

#include <map>
#include <string>
#include <vector>

#include <boost/enable_shared_from_this.hpp>
#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>

#include "LoggerProxy.h"

namespace rsc {
namespace logging {

class LoggerTreeNode;
typedef boost::shared_ptr<LoggerTreeNode> LoggerTreeNodePtr;
typedef boost::weak_ptr<LoggerTreeNode> LoggerTreeNodeWeakPtr;

/**
 * A simple tree representation for loggers.
 *
 * We do not use any special library but create our own one because all publicly
 * available libs like BGL are too big for this purpose and BGL is often not
 * included in boost distributions.
 *
 * @author jwienke
 */
class LoggerTreeNode: public boost::enable_shared_from_this<LoggerTreeNode> {
public:

    /**
     * A unique representation of a name. From the string representation this
     * is the name split at each '.' char. Empty vector means root logger.
     */
    typedef std::vector<std::string> NamePath;

    /**
     * Creates a new node without an assigned LoggerProxy. This needs to be set
     * afterwards with #setLoggerProxy.
     *
     * @param name name of the current path element, i.e. not the full path
     * @param parent parent tree node
     */
    LoggerTreeNode(const std::string& name, LoggerTreeNodeWeakPtr parent);

    /**
     * Creates a new node with a LoggerProxy assigned.
     *
     * @param name name of the current path element, i.e. not the full path
     * @param loggerProxy assigned logger proxy
     * @param parent parent tree node
     */
    LoggerTreeNode(const std::string& name, LoggerProxyPtr loggerProxy,
            LoggerTreeNodeWeakPtr parent);

    LoggerTreeNodePtr getParent() const;

    std::string getName() const;

    LoggerProxyPtr getLoggerProxy() const;

    void setLoggerProxy(LoggerProxyPtr loggerProxy);

    /**
     * Adds a child if it does not exist so far.
     *
     * @param child child to add
     * @return @c true if the new child was added, @c false if it already exited.
     */
    bool addChild(LoggerTreeNodePtr child);

    typedef boost::function<
            LoggerProxyPtr(const NamePath& name, LoggerTreeNodePtr node)> CreateFunction;

    /**
     * Retrieves an (indirect) child and creates required ancestors of this
     * child using a custom callback method.
     *
     * @param path path of the child starting from this node
     * @param createFn function used to create new children
     * @param processedPath the path of this node. Passed to the create function
     * @return the deepest child in the path
     */
    LoggerTreeNodePtr addChildren(const NamePath& path,
            CreateFunction createFn, const NamePath &processedPath = NamePath());

    /**
     * Visitor interface to operate on the tree.
     *
     * @author jwienke
     */
    class Visitor {
    public:
        virtual ~Visitor();

        /**
         * Called for each node in the tree.
         *
         * @param path current path in the tree
         * @param node currently visited node
         * @param parentLevel direct or derived level of the parent node
         * @return @true to descend into this node's children, else @c false
         */
        virtual bool visit(const NamePath& path, LoggerTreeNodePtr node, const Logger::Level& parentLevel) = 0;

    };
    typedef boost::shared_ptr<Visitor> VisitorPtr;

    /**
     * Visits every sub-node excluding this node. Depth-first strategy is used.
     *
     * @param visitor visitor to use
     */
    void visit(VisitorPtr visitor, const NamePath& thisPath = NamePath());

    bool hasChild(const std::string& name) const;

    bool hasChild(const NamePath& path) const;

    LoggerTreeNodePtr getChild(const std::string& name) const;

    LoggerTreeNodePtr getChild(const NamePath& path) const;

    void clearChildren();

    static NamePath nameToPath(const std::string& name);
    static std::string pathToName(const NamePath& path);

    boost::shared_ptr<Logger::Level> getAssignedLevel() const;
    void setAssignedLevel(boost::shared_ptr<Logger::Level> level);
    void setAssignedLevel(const Logger::Level& level);
    bool hasAssignedLevel() const;

private:

    /**
     * New name part of this node. Somehow mirrored from loggerProxy because we
     * do not want to rely on the custom logger implementation correctness.
     */
    std::string name;
    LoggerProxyPtr loggerProxy;

    LoggerTreeNodeWeakPtr parent;
    std::map<std::string, LoggerTreeNodePtr> children;

    boost::shared_ptr<Logger::Level> assignedLevel;

};

}
}

