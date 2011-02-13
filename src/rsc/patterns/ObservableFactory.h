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

#include <boost/signals.hpp>

#include "Factory.h"

namespace rsc {
namespace patterns {

/**
 * A specialized factory class objects of which emit signals when
 * implementations are registered or unregistered.
 *
 * @author Jan Moringen <jmoringe@techfak.uni-bielefeld.de>
 */
template<typename Key, typename Interface>
class ObservableFactory: public Factory<Key, Interface> {
protected:
    typedef Factory<Key, Interface> base;
public:
    typedef typename base::CreateFunction CreateFunction;

    typedef boost::signal2<void, const std::string&, const CreateFunction&>
            ImplAddedSignal;

    typedef boost::signal2<void, const std::string&, const CreateFunction&>
            ImplRemovedSignal;

    /**
     * Return the "implementation added" signal.
     */
    ImplAddedSignal&
    signalImplAdded() throw ();

    /**
     * Return the "implementation removed" signal.
     */
    ImplRemovedSignal&
    signalImplRemoved() throw ();
protected:
    typedef typename base::ImplMap ImplMap;

    ImplAddedSignal signal_impl_added_;
    ImplRemovedSignal signal_impl_removed_;

    /**
     * @throw std::invalid_argument
     */
    void register_(const Key& key, const CreateFunction& create_function_);

    /**
     * @throw NoSuchImpl
     */
    void unregister(const Key& key);
};

/**
 * An observable factory of which at most one instance exists at any time.
 */
template<typename Key, typename Interface>
class ObservableSingletonFactory: public Singleton<ObservableSingletonFactory<
        Key, Interface> > , public ObservableFactory<Key, Interface> {
    friend class Singleton<SingletonFactory<Key, Interface> > ;
private:
    ObservableSingletonFactory();
};

// ObservableFactory implementation

template<typename Key, typename Interface>
typename ObservableFactory<Key, Interface>::ImplAddedSignal&
ObservableFactory<Key, Interface>::signalImplAdded() throw () {
    return this->signal_impl_added_;
}

template<typename Key, typename Interface>
typename ObservableFactory<Key, Interface>::ImplRemovedSignal&
ObservableFactory<Key, Interface>::signalImplRemoved() throw () {
    return this->signal_impl_removed_;
}

template<typename Key, typename Interface>
void ObservableFactory<Key, Interface>::register_(const Key& key,
        const CreateFunction& create_function_) {
    base::register_(key, create_function_);

    //
    if (!this->signal_impl_added_.empty()) {
        this->signal_impl_added_(key, create_function_);
    }
}

template<typename Key, typename Interface>
void ObservableFactory<Key, Interface>::unregister(const Key& key) {
    //
    if (!this->signal_impl_removed_.empty()) {
        typename ImplMap::iterator it;
        if ((it = this->impl_map_.find(key)) != this->impl_map_.end()) {
            this->signal_impl_removed_(it->first, it->second);
	}
    }

    base::unregister(key);
}

// ObservableSingletonFactory implementation

template<typename Key, typename Interface>
ObservableSingletonFactory<Key, Interface>::ObservableSingletonFactory() {
}

}
}
