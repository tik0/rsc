/* ============================================================
 *
 * This file is part of the RSC project
 *
 * Copyright (C) 2010, 2011 Jan Moringen
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

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

namespace rsc { namespace patterns {

  /** @brief This template class implements the singleton pattern.
   *
   * To add singleton behavior to a class @c C, add @c Singleton<C> to
   * its list of base classes.
   *
   * @note C has to contain a friend declaration for @c Singleton<C>.
   * @note This singleton implementation is not thread-safe.
   *
   * @author Jan Moringen <jmoringe@techfak.uni-bielefeld.de>
   */
  template <typename T>
  class Singleton : private boost::noncopyable {
  public:
    /** @brief Retrieve the singleton instance, creating it if
     * necessary.
     *
     * @return A reference to the instance.
     */
    static T&
    getInstance();

    /** @brief This function can be used to make sure the instance is
     * deleted at a particular time.
     *
     * You may need this function to enforce a certain order of
     * destruction.
     *
     * @note The instance will be destroyed in any case but the order
     * of destruction is unspecified then.
     */
    static void
    killInstance();
  private:
    static boost::shared_ptr<T>&
    getStorage();
  };

  // Singleton implementation

  template <typename T>
  T&
  Singleton<T>::getInstance() {
    boost::shared_ptr<T>& instance = getStorage();

    if (!instance)
      instance = boost::shared_ptr<T>(new T());

    return *instance;
  }

  template <typename T>
  void
  Singleton<T>::killInstance() {
    boost::shared_ptr<T>& instance = getStorage();

    if (instance)
      instance.reset();
  }

  template <typename T>
  boost::shared_ptr<T>&
  Singleton<T>::getStorage() {
    static boost::shared_ptr<T> instance
      = boost::shared_ptr<T>(reinterpret_cast<T*>(0));

    return instance;
  }

} }
