#ifndef C3_JS_ELEMENT_HPP
#define C3_JS_ELEMENT_HPP

#include "c3-js-attr.hpp"
#include "c3-js-domtokenlist.hpp"
#include "c3-js-htmlcollection.hpp"
#include "c3-js-namednodemap.hpp"
#include "c3-js-node.hpp"
#include "c3-js-nodelist.hpp"
#include "c3-js-nondocumenttypechildnode.hpp"
#include "c3-js-parentnode.hpp"

struct Element : virtual Node,
                 virtual NonDocumentTypeChildNode,
                 virtual ParentNode {
  using Node::Node;
  NamedNodeMap attributes() const { return {node["attributes"]}; }
  DOMTokenList classList() const { return {node["classList"]}; }
  DOMString className() const { return node["className"].as<DOMString>(); }
  void className(const DOMString &name) { node.set("className", name); }
  DOMString computedName() const {
    return node["computedName"].as<DOMString>();
  }
  DOMString computedRole() const {
    return node["computedRole"].as<DOMString>();
  }
  DOMString id() const { return node["id"].as<DOMString>(); }
  void id(const DOMString &i) { return node.set("id", i); }
  DOMString innerHTML() const { return node["innerHTML"].as<DOMString>(); }
  void innerHTML(const DOMString &html) { return node.set("innerHTML", html); }
  DOMString localName() const { return node["localName"].as<DOMString>(); }
  DOMString namespaceURI() const {
    return node["namespaceURI"].as<DOMString>();
  }
  DOMString prefix() const { return node["prefix"].as<DOMString>(); }
  DOMString tagName() const { return node["tagName"].as<DOMString>(); }

  DOMString getAttribute(const DOMString &name) const {
    return node.call<DOMString>("getAttribute", val(name));
  }
  DOMString getAttributeNS(const DOMString &nameSpace,
                           const DOMString &name) const {
    return node.call<DOMString>("getAttributeNS", val(nameSpace), val(name));
  }
  vector<DOMString> getAttributeNames() const {
    return vecFromJSArray<DOMString>(node.call<val>("getAttributeNames"));
  }
  HTMLCollection getElementsByClassName(const DOMString &names) const {
    return {node.call<val>("getElementsByClassName", val(names))};
  }
  HTMLCollection getElementsByTagName(const DOMString &tag) const {
    return {node.call<val>("getElementsByTagName", val(tag))};
  }
  HTMLCollection getElementsByTagNameNS(const DOMString &nameSpace,
                                        const DOMString &tag) const {
    return {node.call<val>("getElementsByTagNameNS", val(nameSpace), val(tag))};
  }
  bool hasAttribute(const DOMString &name) const {
    return node.call<bool>("hasAttribute", val(name));
  }
  bool hasAttributeNS(const DOMString &nameSpace, const DOMString &name) const {
    return node.call<bool>("hasAttributeNS", val(nameSpace), val(name));
  }
  bool hasAttributes() const { return node.call<bool>("hasAttributes"); }
  void removeAttribute(const DOMString &name) const {
    return node.call<void>("removeAttribute", val(name));
  }
  void removeAttributeNS(const DOMString &nameSpace,
                         const DOMString &name) const {
    return node.call<void>("removeAttributeNS", val(nameSpace), val(name));
  }
  bool setAttribute(const DOMString &name, const DOMString &value) const {
    return node.call<bool>("setAttribute", val(name), val(value));
  }
  bool setAttributeNS(const DOMString &nameSpace, const DOMString &name,
                      const DOMString &value) const {
    return node.call<bool>("setAttributeNS", val(nameSpace), val(name),
                           val(value));
  }
};

#endif
