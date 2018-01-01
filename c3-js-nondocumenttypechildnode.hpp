#ifndef C3_JS_NONDOCUMENTTYPECHILDNODE_HPP
#define C3_JS_NONDOCUMENTTYPECHILDNODE_HPP

#include "c3-js-node.hpp"

struct NonDocumentTypeChildNode : virtual Node {
  using Node::Node;
  template <typename E = Element> E previousElementSibling() const {
    return {node["previousElementSibling"]};
  }
  template <typename E = Element> E nextElementSibling() const {
    return {node["nextElementSibling"]};
  }
};

#endif
