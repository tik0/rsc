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

#ifndef REGISTRY_H_
#define REGISTRY_H_

#include <iostream>
#include <map>
#include <set>
#include <stdexcept>
#include <string>

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/static_assert.hpp>
#include <boost/thread/recursive_mutex.hpp>

#include "rsc/config.h"

namespace rsc {
namespace misc {

/**
 * A templatized global registry. There exists exactly on registry per type at
 * runtime (singleton). Classes to register must implement a method:
 * <code>std::string getRegistryKey() const;</code> that returns a unique key
 * to use for registration.
 *
 * To create a registree, that will be registered on application startup before
 * main use the provided macro. This will work on every supported compiler.
 *
 * This class is reentrant.
 *
 * @author jwienke
 * @tparam R type to register called registree.
 */
template<class R>
class Registry: public boost::noncopyable {

private:

    /**
     * Do not allow to make instances of this class.
     */
    Registry() {
    }

public:

    /**
     * Get singleton instance.
     *
     * @return singleton instance.
     */
    static Registry<R> *instance() {
        static Registry<R> *inst = new Registry<R> ;
        return inst;
    }

    /**
     * Registers a new registree in the Registry. This class takes over the
     * ownership of the pointer and manages deleting the pointer when needed.
     *
     * @param r new registree to register
     * @throw std::invalid_argument if a registree with the same key is already
     *                              registered. In this case @c r is not owned
     *                              by this registry
     */
    void addRegistree(R *r) {

        boost::recursive_mutex::scoped_lock lock(mutex);

        if (registreesByName.count(r->getRegistryKey())) {
            throw std::invalid_argument(
                    "There already is a registree with key '"
                            + r->getRegistryKey() + "'.");
        }

        registreesByName[r->getRegistryKey()] = boost::shared_ptr<R>(r);

    }

    /**
     * Returns the registree for the provided key.
     *
     * @param key key of the registree
     * @return registree
     * @throw std::invalid_argument if there is no registree with this name
     */
    boost::shared_ptr<R> getRegistree(const std::string &key) {

        boost::recursive_mutex::scoped_lock lock(mutex);

        typename std::map<std::string, boost::shared_ptr<R> >::const_iterator
                it = registreesByName.find(key);
        if (it == registreesByName.end()) {
            throw std::invalid_argument("There is no registree with key '"
                    + key + "'.");
        }

        return it->second;

    }

    /**
     * Returns a set with all known registree keys.
     *
     * @return all known registree keys
     */
    std::set<std::string> getKnownRegistryKeys() {

        boost::recursive_mutex::scoped_lock lock(mutex);

        std::set<std::string> keys;
        typename std::map<std::string, boost::shared_ptr<R> >::const_iterator
                it;
        for (it = registreesByName.begin(); it != registreesByName.end(); ++it) {
            keys.insert(it->first);
        }
        return keys;

    }

    /**
     * Removes a registree from this registry. Use this with caution as many
     * clients will assume that system configuration knowledge is static!
     *
     * @param name name of the registree to remove
     * @return @true if a registree was removed, else @c false
     */
    bool removeRegistree(const std::string &name) {
        boost::recursive_mutex::scoped_lock lock(mutex);
        return registreesByName.erase(name);
    }

    /**
     * Returns a recursive mutex that can be used to make multiple operations
     * on this class atomic.
     *
     * @return mutex internally used by this class
     */
    boost::recursive_mutex *getMutex() {
        return &mutex;
    }

private:

    boost::recursive_mutex mutex;
    std::map<std::string, boost::shared_ptr<R> > registreesByName;

    static Registry<R> *inst;

};

#if defined(RSC_HAVE_INIT_METHOD_ATTRIBUTE_CONSTRUCTOR)

/**
 * Creates an object that globally registers in the Registry. This method only
 * works directly in binaries or shared libraries. GCC only handles the
 * constructor keyword in static libraries if the *.o file is really used by the
 * binary. Thus it is advisable not to use static libraries for this purpose.
 *
 * Class names for this macro must be given without namespaces and templates.
 * Create typedefs as required to match these restrictions.
 *
 * @param registryClassName type of the Registry to register in
 * @param registreeClassName concrete registree to register. Must have a default
 *                           constructor.
 */
#define CREATE_GLOBAL_REGISTREE(registryClassName, registreeClassName) \
    __attribute__ ((constructor)) \
    static void init##registreeClassName##In##registryClassName() { \
        ::rsc::misc::Registry<registryClassName>::instance()->addRegistree(new registreeClassName); \
    }

#define CREATE_REGISTRY(registryClassName)

#else

#if defined(RSC_HAVE_INIT_METHOD_CRT)

/**
 * Ensure that getRegistryKey returns no static member. Damn Windows!
 */
#define CREATE_GLOBAL_REGISTREE(registryClassName, registreeClassName) \
	typedef int preInitCallback(void); \
    int init##registreeClassName##In##registryClassName() { \
        ::std::cout << "Registering registree " << #registreeClassName << " at Registry " << ::rsc::misc::Registry<registryClassName>::instance() << ::std::endl; \
        ::rsc::misc::Registry<registryClassName>::instance()->addRegistree(new registreeClassName); \
        return 0; \
    } \
    __pragma(data_seg(".CRT$XCU")) \
    static preInitCallback *autostart##registreeClassName##In##registryClassName[] = { init##registreeClassName##In##registryClassName }; \
    __pragma(data_seg())

#define CREATE_REGISTRY(registryClassName) \
	template __declspec(dllexport) class ::rsc::misc::Registry<registryClassName>;

#else
// There is no way to achieve what we need. Sad :(
BOOST_STATIC_ASSERT(false);
#endif

#endif

}
}

#endif /* REGISTRY_H_ */
