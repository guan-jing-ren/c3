#ifndef C3_JS_RANGE_HPP
#define C3_JS_RANGE_HPP

#include "c3-js-documentfragment.hpp"
#include "c3-js-node.hpp"

struct Range {
  val range = val::null();

  bool collapsed() const { return range["collapsed"].as<bool>(); }
  Node commonAncestorContainer() const {
    return {range["commonAncestorContainer"]};
  }
  Node endContainer() const { return {range["endContainer"]}; }
  unsigned endOffset() const { return range["endOffset"].as<unsigned>(); }
  Node startContainer() const { return {range["startContainer"]}; }
  unsigned startOffset() const { return range["startOffset"].as<unsigned>(); }

  void setStart(const Node &n, unsigned offset) const {
    range.call<void>("setStart", n.node, val(offset));
  }
  void setEnd(const Node &n, unsigned offset) const {
    range.call<void>("setEnd", n.node, val(offset));
  }
  void setStartBefore(const Node &n) const {
    range.call<void>("setStartBefore", n.node);
  }
  void setStartAfter(const Node &n) const {
    range.call<void>("setStartAfter", n.node);
  }
  void setEndBefore(const Node &n) const {
    range.call<void>("setEndBefore", n.node);
  }
  void setEndAfter(const Node &n) const {
    range.call<void>("setEndAfter", n.node);
  }
  void selectNode(const Node &n) const {
    range.call<void>("selectNode", n.node);
  }
  void selectNodeContents(const Node &n) const {
    range.call<void>("selectNodeContents", n.node);
  }
  void collapse(bool c = false) const { range.call<void>("collapse", val(c)); }
  DocumentFragment cloneContents() const {
    return {range.call<val>("cloneContents")};
  }
  void deleteContents() const { range.call<val>("deleteContents"); }
  DocumentFragment extractContents() const {
    return {range.call<val>("extractContents")};
  }
  void insertNode(const Node &n) const {
    range.call<void>("insertNode", n.node);
  }
  void surroundContents(const Node &n) const {
    range.call<void>("surroundContents", n.node);
  }
  int compareBoundaryPoints(val how, const Range &r) const {
    return range.call<int>("compareBoundaryPoints", how, r.range);
  }
  Range cloneRange() const { return {range.call<val>("cloneRange")}; }
  void detach() const { range.call<val>("detach"); }
  DOMString toString() const { return range.call<DOMString>("toString"); }
};

#endif
