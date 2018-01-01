#ifndef C3_JS_HTMLCOLLECTION_HPP
#define C3_JS_HTMLCOLLECTION_HPP

#include "c3-js-fwd.hpp"
#include "c3-js-node.hpp"

struct HTMLCollection {
  val collection = val::null();

  struct iterator {
    using iterator_category = forward_iterator_tag;
    using difference_type = int;
    using value_type = Node;
    using pointer = iterator;
    using reference = value_type;

    const HTMLCollection *collection;
    int i;

    iterator &operator++() {
      ++i;
      return *this;
    }

    Node operator*() const { return collection->item(i); }

    bool operator!=(const iterator &other) const {
      return i != other.i || collection != other.collection;
    }
  };

  int length() const { return collection["length"].as<int>(); }
  Node item(int i) const { return {collection.call<val>("item", val(i))}; }
  Node namedItem(const string &name) const {
    return {collection.call<val>("namedIterm", val(name))};
  }

  iterator begin() const { return {this, 0}; }
  iterator end() const { return {this, length()}; }

  template <typename A = allocator<Node>> operator vector<Node, A>() const {
    return {begin(), end()};
  }
};

#endif
