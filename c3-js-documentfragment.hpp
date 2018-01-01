#ifndef C3_JS_DOCUMENTFRAGMENT_HPP
#define C3_JS_DOCUMENTFRAGMENT_HPP

#include "c3-js-element.hpp"
#include "c3-js-node.hpp"
#include "c3-js-parentnode.hpp"

struct DocumentFragment : virtual Node, virtual ParentNode {
  using Node::Node;

  Element getElementById(const DOMString &id) const {
    return {node.call<val>("getElementBy")};
  }
};

#endif
