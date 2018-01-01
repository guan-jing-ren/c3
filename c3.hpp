#ifndef C3_HPP
#define C3_HPP

#include "c3-select-select.hpp"

struct c3 {
  template <typename C> Select select(C &&c) const {
    return {forward<C>(c), false};
  }
  template <typename C> Select selectAll(C &&c) const {
    return {forward<C>(c), true};
  }
};

#endif
