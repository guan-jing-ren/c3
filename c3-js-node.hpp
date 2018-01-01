#ifndef C3_JS_NODE_HPP
#define C3_JS_NODE_HPP

#include "c3-js-fwd.hpp"

struct Node {
  val node = val::null();

  Node() = default;
  Node(val n) : node(n) {}
  Node(const Node &) = default;
  Node(Node &&) = default;
  Node &operator=(const Node &) = default;
  Node &operator=(Node &&) = default;
  ~Node() = default;

  DOMString baseURI() const { return node["baseURI"].as<DOMString>(); }
  template <typename N = NodeList> N childNodes() const {
    return {node["childNodes"]};
  }
  Node firstChild() const { return {node["firstChild"]}; }
  Node lastChild() const { return {node["lastChild"]}; }
  Node nextSibling() const { return {node["nextSibling"]}; }
  DOMString nodeName() const { return node["nodeName"].as<DOMString>(); }
  unsigned short nodeType() const {
    return node["nodeType"].as<unsigned short>();
  }
  DOMString nodeValue() const { return node["nodeValue"].as<DOMString>(); }
  void nodeValue(const DOMString &value) { node.set("nodeValue", val(value)); }
  template <typename D = Document> D ownerDocument() const {
    return {node["ownerDocument"]};
  }
  Node parentNode() const { return {node["parentNode"]}; }
  template <typename E = Element> E parentElement() const {
    return {node["parentElement"]};
  }
  Node previousSibling() const { return {node["previousSibling"]}; }
  DOMString textContent() const { return node["textContent"].as<DOMString>(); }
  void textContent(const DOMString &value) {
    node.set("textContent", val(value));
  }

  Node appendChild(const Node &n) const {
    return {node.call<val>("appendChild", n.node)};
  }
  Node cloneNode(bool deep = true) const {
    return {node.call<val>("cloneNode", val(deep))};
  }
  unsigned compareDocumentPosition(const Node &n) const {
    return node.call<unsigned>("compareDocumentPosition", n.node);
  }
  bool contains(const Node &n) const {
    return node.call<bool>("contains", n.node);
  }
  bool hasChildNodes() const { return node.call<bool>("hasChildNodes"); }
  Node insertBefore(const Node &n, const Node &ref) const {
    return {node.call<val>("insertBefore", n.node, ref.node)};
  }
  bool isDefaultNamespace(const DOMString &namespaceURI) const {
    return node.call<bool>("isDefaultNamespace", val(namespaceURI));
  }
  bool isEqualNode(const Node &n) const {
    return node.call<bool>("isEqualNode", n.node);
  }
  bool isSameNode(const Node &n) const {
    return node.call<bool>("isSameNode", n.node);
  }
  DOMString lookupPrefix() const {
    return node.call<DOMString>("lookupPrefix");
  }
  DOMString lookupNamespaceURI(const DOMString &prefix) const {
    return node.call<DOMString>("lookupPrefix", val(prefix));
  }
  void normalize() const { node.call<void>("normalize"); }
  Node removeChild(const Node &n) const {
    return {node.call<val>("removeChild", n.node)};
  }
  Node replaceChild(const Node &n, const Node &old) const {
    return {node.call<val>("replaceChild", n.node, old.node)};
  }
};

#endif
