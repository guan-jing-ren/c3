#ifndef C3_JS_TREEWALKER_HPP
#define C3_JS_TREEWALKER_HPP

#include "c3-js-node.hpp"
#include "c3-js-nodefilter.hpp"

struct TreeWalker {
  val walker = val::null();

  Node root() const { return {walker["root"]}; }
  unsigned long whatToShow() const {
    return walker["whatToShow"].as<unsigned long>();
  }
  NodeFilter filter() const { return {walker["filter"]}; }
  Node currentNode() const { return {walker["currentNode"]}; }

  Node parentNode() const { return {walker.call<val>("parentNode")}; }
  Node firstChild() const { return {walker.call<val>("firstChild")}; }
  Node lastChild() const { return {walker.call<val>("lastChild")}; }
  Node previousSibling() const { return {walker.call<val>("previousSibling")}; }
  Node nextSibling() const { return {walker.call<val>("nextSibling")}; }
  Node previousNode() const { return {walker.call<val>("previousNode")}; }
  Node nextNode() const { return {walker.call<val>("nextNode")}; }
};

#endif
