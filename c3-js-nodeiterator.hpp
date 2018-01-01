#include "c3-js-node.hpp"
#include "c3-js-nodefilter.hpp"

struct NodeIterator {
  val iterator = val::null();

  Node root() const { return {iterator["root"]}; }
  unsigned long whatToShow() const {
    return iterator["whatToShow"].as<unsigned long>();
  }
  NodeFilter filter() const { return {iterator["filter"]}; }

  Node previousNode() const { return {iterator.call<val>("previousNode")}; }
  Node nextNode() const { return {iterator.call<val>("nextNode")}; }
};
