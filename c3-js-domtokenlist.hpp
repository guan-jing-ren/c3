#ifndef C3_JS_DOMTOKENLIST_HPP
#define C3_JS_DOMTOKENLIST_HPP

#include "c3-js-fwd.hpp"
#include "c3-js-iterator.hpp"

struct DOMTokenList {
  val list = val::null();

  struct iterator {
    using iterator_category = forward_iterator_tag;
    using difference_type = int;
    using value_type = Node;
    using pointer = iterator;
    using reference = value_type;

    const DOMTokenList *list;
    int i;

    iterator &operator++() {
      ++i;
      return *this;
    }

    template <typename N = Node> N operator*() const {
      return list->item<N>(i);
    }

    bool operator!=(const iterator &other) const {
      return i != other.i || list != other.list;
    }
  };

  int length() const { return list["length"].as<int>(); }
  DOMString value() const { return list["value"].as<DOMString>(); }
  template <typename N = Node> N item(int i) const {
    return {list.call<val>("item", val(i))};
  }

  iterator begin() const { return {this, 0}; }
  iterator end() const { return {this, length()}; }

  Iterator<val> entries() const { return {list.call<val>("entries")}; }
  Iterator<unsigned> keys() const { return {list.call<val>("keys")}; };
  Iterator<DOMString> values() const { return {list.call<val>("values")}; }

  template <typename F, typename N = Node> void forEach(F &&f) {
    for_each(begin(), end(), f);
  }

  template <typename N = Node, typename A = allocator<N>>
  operator vector<N, A>() const {
    return {begin(), end()};
  }

  bool contains(const DOMString &token) const {
    return list.call<bool>("contains", val(token));
  }

  void add(const DOMString &token) const { list.call<void>("add", val(token)); }

  void remove(const DOMString &token) const {
    list.call<void>("remove", val(token));
  }

  void replace(const DOMString &old, const DOMString &new_) const {
    list.call<void>("replace", val(old), val(new_));
  }

  bool toggle(const DOMString &token, optional<bool> force = nullopt) const {
    if (force)
      return list.call<bool>("toggle", val(token), *force);
    else
      return list.call<bool>("toggle", val(token));
  }
};

#endif
