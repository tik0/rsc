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
#include <boost/iterator/transform_iterator.hpp>

#include "rsc/patterns/NoSuchKey.h"
#include "rsc/patterns/Accessors.h"
#include "rsc/patterns/detail/ForceConst.h"
#include "rsc/runtime/TypeStringTools.h"

namespace rsc {
namespace patterns {

/** @brief
 *
 * @note The container type @a Container has to be a model of the @a
 * Container concept defined in the stl.
 * @note The type container_proxy<Container> is almost a model of
 * the @a Container concept, but it is not a model of the @a
 * Assignable concept.
 *
 * @author Jan Moringen <jmoringe@techfak.uni-bielefeld.de>
 */
template<typename Container, typename Accessor = pass_through>
class ContainerProxy: private boost::noncopyable {
public:
    typedef typename Container::size_type size_type;

    typedef typename Container::value_type value_type;
    typedef typename Container::pointer pointer;

    typedef typename Container::const_reference const_reference;
    typedef typename Container::reference reference;

    typedef boost::transform_iterator<Accessor,
            typename Container::const_iterator, typename detail::force_const<
                    typename Accessor::result_type>::type> const_iterator;

    typedef boost::transform_iterator<Accessor, typename Container::iterator,
            typename Accessor::result_type> iterator;

    typedef typename iterator::difference_type difference_type;

    /*! @brief
     *
     * @param container
     */
    ContainerProxy(Container& container);

    /*! @brief
     *
     * @param container
     * @param p1
     */
    template<typename P1> // TODO
    ContainerProxy(Container& container, P1 p1);

    /*! @brief
     *
     * @param container
     * @param p1
     * @param p2
     */
    template<typename P1, typename P2> // TODO
    ContainerProxy(Container& container, P1 p1, P2 p2);

    const_iterator
    begin() const throw ();

    iterator
    begin() throw ();

    const_iterator
    end() const throw ();

    iterator
    end() throw ();

    size_type
    size() const throw ();

    bool
    empty() const throw ();
protected:
    typedef Container container_type;

    typedef Accessor accessor_type;

    container_type& container;

    accessor_type accessor;
};

template<typename Container>
class ContainerProxy<Container, pass_through> {
public:
    typedef typename Container::size_type size_type;

    typedef typename Container::value_type value_type;
    typedef typename Container::pointer pointer;

    typedef typename Container::const_iterator const_iterator;
    typedef typename Container::iterator iterator;

    typedef typename Container::const_reference const_reference;
    typedef typename Container::reference reference;

    typedef typename Container::difference_type difference_type;

    ContainerProxy(Container& container);

    inline const_iterator
    begin() const throw ();

    inline iterator
    begin() throw ();

    inline const_iterator
    end() const throw ();

    inline iterator
    end() throw ();

    inline size_type
    size() const throw ();

    inline bool
    empty() const throw ();
protected:
    typedef Container container_type;

    typedef pass_through accessor_type;

    container_type& container;
};

// ContainerProxy implementation

template<typename Container, typename Accessor>
ContainerProxy<Container, Accessor>::ContainerProxy(Container& container) :
    container(container) {
}

template<typename Container, typename Accessor>
typename ContainerProxy<Container, Accessor>::const_iterator ContainerProxy<
        Container, Accessor>::begin() const throw () {
    return const_iterator(this->container.begin(), this->accessor);
}

template<typename Container, typename Accessor>
typename ContainerProxy<Container, Accessor>::iterator ContainerProxy<
        Container, Accessor>::begin() throw () {
    return iterator(this->container.begin(), this->accessor);
}

template<typename Container, typename Accessor>
typename ContainerProxy<Container, Accessor>::const_iterator ContainerProxy<
        Container, Accessor>::end() const throw () {
    return const_iterator(this->container.end(), this->accessor);
}

template<typename Container, typename Accessor>
typename ContainerProxy<Container, Accessor>::iterator ContainerProxy<
        Container, Accessor>::end() throw () {
    return iterator(this->container.end(), this->accessor);
}

template<typename Container, typename Accessor>
typename ContainerProxy<Container, Accessor>::size_type ContainerProxy<
        Container, Accessor>::size() const throw () {
    return this->container.size();
}

template<typename Container, typename Accessor>
bool ContainerProxy<Container, Accessor>::empty() const throw () {
    return this->container.empty();
}

// ContainerProxy<Container, pass_through> implementation

template<typename Container>
ContainerProxy<Container, pass_through>::ContainerProxy(Container& container) :
    container(container) {
}

template<typename Container>
typename ContainerProxy<Container, pass_through>::const_iterator ContainerProxy<
        Container, pass_through>::begin() const throw () {
    return container.begin();
}

template<typename Container>
typename ContainerProxy<Container, pass_through>::iterator ContainerProxy<
        Container, pass_through>::begin() throw () {
    return container.begin();
}

template<typename Container>
typename ContainerProxy<Container, pass_through>::const_iterator ContainerProxy<
        Container, pass_through>::end() const throw () {
    return container.end();
}

template<typename Container>
typename ContainerProxy<Container, pass_through>::iterator ContainerProxy<
        Container, pass_through>::end() throw () {
    return container.end();
}

template<typename Container>
typename ContainerProxy<Container, pass_through>::size_type ContainerProxy<
        Container, pass_through>::size() const throw () {
    return container.size();
}

template<typename Container>
bool ContainerProxy<Container, pass_through>::empty() const throw () {
    return container.empty();
}

}
}
