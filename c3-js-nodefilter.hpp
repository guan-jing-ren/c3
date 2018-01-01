#ifndef C3_JS_NODEFILTER_HPP
#define C3_JS_NODEFILTER_HPP

#include "c3-js-node.hpp"

struct NodeFilter {
  val filter = val::null();

  val acceptNode(const Node &node) const {
    return filter.call<val>("acceptNode", node.node);
  }
};

#endif
