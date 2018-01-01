#ifndef C3_JS_NODELIST_HPP
#define C3_JS_NODELIST_HPP

#include "c3-js-fwd.hpp"
#include "c3-js-iterator.hpp"
#include "c3-js-node.hpp"

struct NodeList {
  val list = val::null();

  struct iterator {
    using iterator_category = forward_iterator_tag;
    using difference_type = int;
    using value_type = Node;
    using pointer = iterator;
    using reference = value_type;

    const NodeList *list;
    int i;

    iterator &operator++() {
      ++i;
      return *this;
    }

    Node operator*() const { return list->item(i); }

    bool operator!=(const iterator &other) const {
      return i != other.i || list != other.list;
    }
  };

  int length() const { return list["length"].as<int>(); }
  Node item(int i) const { return {list.call<val>("item", val(i))}; }

  iterator begin() const { return {this, 0}; }
  iterator end() const { return {this, length()}; }

  Iterator<Node> entries() const { return {list.call<val>("entries")}; }
  Iterator<unsigned> keys() const { return {list.call<val>("keys")}; };
  Iterator<Node> values() const { return {list.call<val>("values")}; }

  template <typename F> void forEach(F &&f) { for_each(begin(), end(), f); }

  template <typename A = allocator<Node>> operator vector<Node, A>() const {
    return {begin(), end()};
  }
};

#endif
