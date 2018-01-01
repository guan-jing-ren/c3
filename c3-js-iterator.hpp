#ifndef C3_JS_ITERATOR_HPP
#define C3_JS_ITERATOR_HPP

#include "c3-js-fwd.hpp"

template <typename T> struct Iterator {
  val iter = val::null();

  struct Result {
    val result = val::null();

    bool done() const { return result["done"].template as<bool>(); }
    T value() const {
      if constexpr (is_constructible_v<T, val>)
        return {result["value"]};
      else
        return result["value"].template as<T>();
    }
  };

  Result next() const { return {iter.call<val>("next")}; };
};

#endif
