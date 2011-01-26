#pragma once

#include <boost/shared_ptr.hpp>

namespace rsc { namespace patterns {

  struct pass_through {
  };

  /**
   * @author Jan Moringen <jmoringe@techfak.uni-bielefeld.de>
   */
  template <typename T>
  struct dereferencer {
    typedef T& result_type;

    inline result_type
    operator()(T* t) const {
      return *t;
    }

    inline const result_type
    operator()(const T* t) const {
      return *t;
    }
  };

  /**
   * @author Jan Moringen <jmoringe@techfak.uni-bielefeld.de>
   */
  template <typename T>
  struct shared_ptr_dereferencer {
    typedef T& result_type;

    inline result_type
    operator()(boost::shared_ptr<T> t) const {
      return *t;
    }

    inline const result_type
    operator()(boost::shared_ptr<const T> t) const {
      return *t;
    }
  };

} }
