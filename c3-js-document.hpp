#ifndef C3_JS_DOCUMENT_HPP
#define C3_JS_DOCUMENT_HPP

#include "c3-js-attr.hpp"
#include "c3-js-comment.hpp"
#include "c3-js-cssstylesheet.hpp"
#include "c3-js-documentfragment.hpp"
#include "c3-js-domimplementation.hpp"
#include "c3-js-element.hpp"
#include "c3-js-htmlcollection.hpp"
#include "c3-js-location.hpp"
#include "c3-js-node.hpp"
#include "c3-js-nodefilter.hpp"
#include "c3-js-nodeiterator.hpp"
#include "c3-js-nodelist.hpp"
#include "c3-js-parentnode.hpp"
#include "c3-js-processinginstruction.hpp"
#include "c3-js-range.hpp"
#include "c3-js-stylesheetlist.hpp"
#include "c3-js-text.hpp"
#include "c3-js-treewalker.hpp"

struct Document : virtual Node, virtual ParentNode {
  using Node::Node;

  Document(val node) : Node(node) {}
  Document() : Document(val::global("document")) {}

  DOMString characterSet() const {
    return node["characterSet"].as<DOMString>();
  }
  DocumentType docType() const { return {node["docType"]}; }
  Element documentElement() const { return {node["documentElement"]}; }
  DOMString documentURI() const { return node["documentURI"].as<DOMString>(); }
  bool hidden() const { return node["hidden"].as<bool>(); }
  DOMImplementation implementation() const { return {node["implementation"]}; }
  DOMString lastStyleSheetSet() const {
    return node["lastStyleSheetSet"].as<DOMString>();
  }
  DOMString preferredStyleSheetSet() const {
    return node["preferredStyleSheetSet"].as<DOMString>();
  }
  DOMString selectedStyleSheetSet() const {
    return node["selectedStyleSheetSet"].as<DOMString>();
  }
  DOMString styleSheetSets() const {
    return node["styleSheetSets"].as<DOMString>();
  }
  StyleSheetList<CSSStyleSheet> styleSheets() const {
    return {node["styleSheets"]};
  }
  DOMString visibilityState() const {
    return node["visibilityState"].as<DOMString>();
  }

  Element activeElement() const { return {node["activeElement"]}; }
  Element body() const { return {node["body"]}; }
  DOMString cookie() const { return node["cookie"].as<DOMString>(); }
  template <typename W = Window> W defaultView() const {
    return {node["cookie"]};
  }
  DOMString designMode() const { return node["designMode"].as<DOMString>(); }
  DOMString dir() const { return node["dir"].as<DOMString>(); }
  DOMString domain() const { return node["domain"].as<DOMString>(); }
  HTMLCollection embeds() const { return {node["embeds"]}; }
  HTMLCollection forms() const { return {node["forms"]}; }
  Element head() const { return {node["head"]}; }
  HTMLCollection images() const { return {node["images"]}; }
  DOMString lastModified() const {
    return node["lastModified"].as<DOMString>();
  }
  HTMLCollection links() const { return {node["links"]}; }
  Location location() const { return {node["location"]}; }
  HTMLCollection plugins() const { return {node["plugins"]}; }
  DOMString readyState() const { return node["readyState"].as<DOMString>(); }
  DOMString referrer() const { return node["referrer"].as<DOMString>(); }
  HTMLCollection scripts() const { return {node["scripts"]}; }
  DOMString title() const { return node["title"].as<DOMString>(); }
  void title(const DOMString &t) { return node.set("title", t); }
  DOMString URL() const { return node["URL"].as<DOMString>(); }

  Node adoptNode(Node n) const { return {node.call<val>("adoptNode", n.node)}; }
  Attr createAttribute(const DOMString &name) const {
    return {node.call<val>("createAttribute", val(name))};
  }
  Attr createAttributeNS(const DOMString &nameSpace,
                         const DOMString &name) const {
    return {node.call<val>("createAttributeNS", val(nameSpace), val(name))};
  }
  Comment createComment(const DOMString &data) const {
    return {node.call<val>("createComment", val(data))};
  }
  DocumentFragment createDocumentFragment() const {
    return {node.call<val>("createDocumentFragment")};
  }
  Element createElement(const DOMString &tagName) const {
    return {node.call<val>("createElement", val(tagName))};
  }
  Element createElementNS(const DOMString &nameSpace,
                          const DOMString &tagName) const {
    return {node.call<val>("createElementNS", val(nameSpace), val(tagName))};
  }
  NodeIterator createNodeIterator(const Node &n,
                                  optional<unsigned long> whatToShow = nullopt,
                                  optional<NodeFilter> filter = nullopt) const {
    if (!whatToShow && !filter)
      return {node.call<val>("createNodeIterator", n.node)};
    else if (!filter)
      return {node.call<val>("createNodeIterator", n.node, val(*whatToShow))};
    else
      return {node.call<val>("createNodeIterator", n.node, val(*whatToShow),
                             filter->filter)};
  }
  ProcessingInstruction
  createProcessingInstruction(const DOMString &target,
                              const DOMString &data) const {
    return {
        node.call<val>("createProcessingInstruction", val(target), val(data))};
  }
  Range createRange() const { return {node.call<val>("createRange")}; }
  Text createTextNode(const DOMString &data) const {
    return {node.call<val>("createTextNode", val(data))};
  }
  TreeWalker createTreeWalker(const Node &n,
                              optional<unsigned long> whatToShow = nullopt,
                              optional<NodeFilter> filter = nullopt) const {
    if (!whatToShow && !filter)
      return {node.call<val>("createTreeWalker", n.node)};
    else if (!filter)
      return {node.call<val>("createTreeWalker", n.node, val(*whatToShow))};
    else
      return {node.call<val>("createTreeWalker", n.node, val(*whatToShow),
                             filter->filter)};
  }
  void enableStyleSheetsForSet(const DOMString &name) const {
    node.call<void>("enableStyleSheetsForSet", val(name));
  }
  HTMLCollection getElementsByClassName(const DOMString &name) const {
    return {node.call<val>("getElementsByClassName", val(name))};
  }
  HTMLCollection getElementsByTagName(const DOMString &name) const {
    return {node.call<val>("getElementsByTagName", val(name))};
  }
  HTMLCollection getElementsByTagNameNS(const DOMString &nameSpace,
                                        const DOMString &name) const {
    return {
        node.call<val>("getElementsByTagNameNS", val(nameSpace), val(name))};
  }
  Node importNode(const Node &n, bool deep = true) const {
    return {node.call<val>("importNode", n.node, deep)};
  }
  Element getElementById(const DOMString &id) const {
    return {node.call<val>("getElementById", val(id))};
  }
  Element querySelector(const DOMString &selector) const {
    return {node.call<val>("querySelector", val(selector))};
  }
  NodeList querySelectorAll(const DOMString &selector) const {
    return {node.call<val>("querySelectorAll", val(selector))};
  }

  void close() const { node.call<val>("close"); }
  NodeList getElementsByName(const DOMString &name) const {
    return {node.call<val>("getElementsByName", val(name))};
  }
  bool hasFocus() const { return node.call<bool>("hasFocus"); }
  void open() const { node.call<val>("open"); }
  void write(const DOMString &text) const {
    node.call<val>("write", val(text));
  }
  void writeln(const DOMString &text) const {
    node.call<val>("writeln", val(text));
  }
};

#endif
