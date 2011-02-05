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

#include <stdexcept>
#include <utility>
#include <algorithm>
#include <map>

#include <boost/function.hpp>
#include <boost/format.hpp>

#include "../runtime/TypeStringTools.h"
#include "../runtime/Properties.h"
#include "Singleton.h"
#include "NoSuchImpl.h"
#include "ConstructError.h"
#include "AssociativeProxy.h"

namespace rsc {
namespace patterns {

/**
 * An interface-independent factory interface, mainly used as a base class for
 * more specific factories.
 *
 * A factory of this kind is basically a mapping of keys to creation
 * functions that create objects of some common base class.
 *
 * The factory has a list of implementations that can be retrieved
 * using the @a impls_base method.
 *
 * Runtime type information for the interface implemented by
 * constructed objects can be retrieved using the
 * @a get_interface_type method.
 *
 * Objects are constructed by calling @a create_base with a key
 * identifying the implementation and properties to be used as
 * arguments to the constructor.
 *
 * @author Jan Moringen <jmoringe@techfak.uni-bielefeld.de>
 */
template<typename Key>
class FactoryBase {
public:
    typedef Key key_type;
    typedef boost::function1<void*, const runtime::Properties&> create_function;
    typedef std::pair<const std::type_info*, void*> type_and_storage;

    typedef std::map<Key, create_function> impl_map;

    class impl_map_proxy: public AssociativeProxy<impl_map> {
    protected:
        template<typename K, typename I>
        friend class Factory;

        typedef AssociativeProxy<impl_map> base;

        impl_map_proxy(impl_map& container) :
            base(container) {
        }
    };

    /**
     * Return the type information of the interface type of the factory.
     */
    virtual const std::type_info&
    get_interface_type() const throw () = 0;

    /**
     * Return a container-like object holding all registered implementations.
     *
     * @return A constant reference to the implementation list proxy.
     */
    virtual const impl_map_proxy&
    impls_base() const throw () = 0;

    /**
     * Create and return an instance of the implementation designated by @a key.
     * @a properties_ is passed to the create function.
     *
     * @param key The name of a registered implementation.
     * @param properties_ A set of properties. The interpretation is up the
     *                    selected create function.
     * @return A pair containing the type information of the created object and
     *         a void pointer pointing to it.
     * @throw NoSuchImpl If @a key does not name a registered implementation.
     * @throw ConstructError If the selected create function produced an
     *                       exception during execution.
     */
    virtual type_and_storage
    create_base(const Key& key, const runtime::Properties& properties_ =
            runtime::Properties()) = 0;
};

/**
 * Objects of this class manage a family of named implementations of a
 * particular interface.
 *
 * @see FactoryBase
 *
 * @author Jan Moringen <jmoringe@techfak.uni-bielefeld.de>
 */
template<typename Key, typename Interface>
class Factory: public FactoryBase<Key> {
public:
    typedef FactoryBase<Key> base;

    typedef typename base::key_type key_type;
    typedef Interface interface_type;

    typedef boost::function1<interface_type*, const runtime::Properties&>
            create_function;

    typedef std::map<Key, create_function> impl_map;

    typedef typename base::impl_map impl_map_base;
    typedef typename base::impl_map_proxy impl_map_base_proxy;

public:

    /**
     * This object presents the registered implementations in a form very
     * similar to a STL container.
     *
     * In addition, implementations can be registered or unregistered using
     * additional functions.
     */
    class impl_map_proxy: public AssociativeProxy<impl_map> {
        friend class Factory<Key, Interface> ;
    public:
        /**
         * @throw std::invalid_argument
         */
        void register_(const key_type& key,
                const create_function& create_function_);

        /**
         * @throw NoSuchImpl
         */
        void unregister(const key_type& key);
    private:
        typedef AssociativeProxy<impl_map> base_type;

        Factory<Key, Interface>& owner;

        impl_map_proxy(Factory<Key, Interface>& owner);
    };

    friend class impl_map_proxy;

    Factory();

    virtual
    ~Factory();

    const std::type_info&
    get_interface_type() const throw ();

    const impl_map_base_proxy&
    impls_base() const throw ();

    /**
     * Return a container-like object holding all registered implementations.
     */
    impl_map_proxy&
    impls() throw ();

    /**
     * Return a container-like object holding all registered implementations.
     */
    const impl_map_proxy&
    impls() const throw ();

    /**
     * @throw NoSuchImpl
     * @throw ConstructError
     */
    typename FactoryBase<Key>::type_and_storage // TODO we should inherit that
    create_base(const Key& key, const runtime::Properties& properties_ =
            runtime::Properties());

    /**
     * Create and return an instance of the implementation designated by @a key.
     * @a properties_ is passed to the create function.
     *
     * @param key The name of a registered implementation.
     * @param properties_ A set of properties. The interpretation is up the
     *        selected create function.
     * @return A pointer to a newly created instance of the implementation
     *         specified by @a key.
     * @throw NoSuchImpl If @a key does not name a registered implementation.
     * @throw ConstructError If the selected create function produced an
     *                       exception during execution.
     */
    Interface*
    create_inst(const Key& key, const runtime::Properties& properties_ =
            runtime::Properties());
protected:
    impl_map_base impl_map_base_;
    impl_map_base_proxy impl_map_base_proxy_;

    impl_map impl_map_;
    impl_map_proxy impl_map_proxy_;

    /**
     * @throw std::invalid_
     */
    virtual void register_(const Key& key,
            const create_function& create_function_);

    /**
     * @throw NoSuchImpl
     */
    virtual void unregister(const Key& key);
};

/**
 * A factory of which at most one instance exists at any time.
 */
template<typename Key, typename Interface>
class SingletonFactory: public Singleton<SingletonFactory<Key, Interface> > ,
        public Factory<Key, Interface> {
    friend class Singleton<SingletonFactory<Key, Interface> > ;
private:
    SingletonFactory();
};

// Factory::impl_list_proxy implementation

template<typename Key, typename Interface>
Factory<Key, Interface>::impl_map_proxy::impl_map_proxy(
        Factory<Key, Interface>& owner) :
    base_type(owner.impl_map_), owner(owner) {
}

template<typename Key, typename Interface>
void Factory<Key, Interface>::impl_map_proxy::register_(const key_type& key,
        const create_function& create_function_) {
    this->owner.register_(key, create_function_);
}

template<typename Key, typename Interface>
void Factory<Key, Interface>::impl_map_proxy::unregister(const key_type& key) {
    this->owner.unregister(key);
}

// Factory implementation

template<typename Key, typename Interface>
Factory<Key, Interface>::Factory() :
    impl_map_base_proxy_(this->impl_map_base_), impl_map_proxy_(*this) {
}

template<typename Key, typename Interface>
Factory<Key, Interface>::~Factory() {
}

template<typename Key, typename Interface>
const std::type_info&
Factory<Key, Interface>::get_interface_type() const throw () {
    return typeid(Interface);
}

template<typename Key, typename Interface>
const typename Factory<Key, Interface>::impl_map_base_proxy&
Factory<Key, Interface>::impls_base() const throw () {
    return this->impl_map_base_proxy_;
}

template<typename Key, typename Interface>
typename Factory<Key, Interface>::impl_map_proxy&
Factory<Key, Interface>::impls() throw () {
    return this->impl_map_proxy_;
}

template<typename Key, typename Interface>
const typename Factory<Key, Interface>::impl_map_proxy&
Factory<Key, Interface>::impls() const throw () {
    return this->impl_map_proxy_;
}

template<typename Key, typename Interface>
void Factory<Key, Interface>::register_(const Key& key,
        const create_function& create_function_) {
    //
    if (this->impl_map_.find(key) != this->impl_map_.end())
        throw std::invalid_argument(runtime::type_string("duplicate key `%1%'",
                "duplicate key", key));

    //
    this->impl_map_base_[key] = create_function_;
    this->impl_map_[key] = create_function_;
}

template<typename Key, typename Interface>
void Factory<Key, Interface>::unregister(const Key& key) {
    //
    typename impl_map::iterator it;
    if ((it = this->impl_map_.find(key)) == this->impl_map_.end()) {
        throw NoSuchImpl(runtime::type_string(
                "no implementation found for specified key `%1%'",
                "no implementation found for specified key", key));
    }

    //
    this->impl_map_base_.erase(key);
    this->impl_map_.erase(it);
}

template<typename Key, typename Interface>
typename FactoryBase<Key>::type_and_storage Factory<Key, Interface>::create_base(
        const Key& key, const runtime::Properties& properties_) {
    Interface* instance = create_inst(key, properties_);

    return std::make_pair(&typeid(*instance), instance);
}

template<typename Key, typename Interface>
Interface*
Factory<Key, Interface>::create_inst(const Key& key,
        const runtime::Properties& properties_) {
    // Try to find the implementation specified by key.
    typename impl_map::const_iterator it;
    if ((it = this->impl_map_.find(key)) == this->impl_map_.end())
        throw NoSuchImpl(runtime::type_string(
                "no implementation found for specified key `%1%'",
                "no implementation found for specified key", key));

    // Try to create an instance of that implementation.
    Interface* instance = 0;
    try {
        instance = reinterpret_cast<Interface*> (it->second(properties_));
    } catch (const std::exception& exception_) {
        throw ConstructError(runtime::type_name(typeid(exception_)) + ": "
                + exception_.what());
    } catch (...) {
        throw ConstructError(runtime::type_string(
                "could not construct implementation instance for key `%1%'",
                "could not construct implementation instance", key));
    }

    // Return the constructed instance.
    return instance;
}

// SingletonFactory implementation

template<typename Key, typename Interface>
SingletonFactory<Key, Interface>::SingletonFactory() {
}

// free function implementations

template<typename Ch, typename Tr, typename Key, typename Interface>
std::basic_ostream<Ch, Tr>&
operator<<(std::basic_ostream<Ch, Tr>& stream,
        const Factory<Key, Interface>& factory) {
    typedef Factory<Key, Interface> Factory_type;
    typedef typename Factory<Key, Interface>::impl_map_proxy
            impl_map_proxy_type;

    //
    stream << (boost::format("implementations of interface %1%:\n")
            % runtime::type_name(typeid(Interface)));

    //
    const impl_map_proxy_type& impls = factory.impls();

    for (typename impl_map_proxy_type::const_iterator it = impls.begin(); it
            != impls.end(); ++it)
        stream << (boost::format("* %1%\n") % it->first);

    // Don't forget to return the stream.
    return stream;
}

}
}
