#ifndef C3_JS_PARENTNODE_HPP
#define C3_JS_PARENTNODE_HPP

#include "c3-js-htmlcollection.hpp"
#include "c3-js-node.hpp"

struct ParentNode : virtual Node {
  HTMLCollection children() const { return {node["children"]}; }
  template <typename E = Element> E firstElementChild() const {
    return {node["firstElementChild"]};
  }
  template <typename E = Element> E lastElementChild() const {
    return {node["lastElementChild"]};
  }
  unsigned long childElementCount() const {
    return node["childElementCount"].as<unsigned long>();
  }

  template <typename E = Element>
  E querySelector(const string &selector) const {
    return {node.call<val>("querySelector", val(selector))};
  }
  NodeList querySelectorAll(const string &selector) const {
    return {node.call<val>("querySelectorAll", val(selector))};
  }
};

#endif
