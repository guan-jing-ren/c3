#ifndef C3_JS_STYLESHEET_HPP
#define C3_JS_STYLESHEET_HPP

#include "c3-js-node.hpp"

struct StyleSheet {
  val sheet = val::null();

  bool disabled() const { return sheet["disabled"].as<bool>(); }
  void disabled(bool d) { return sheet.set("disabled", d); }
  DOMString href() const { return sheet["href"].as<DOMString>(); }
  DOMString media() const { return sheet["media"].as<DOMString>(); }
  void media(const DOMString &m) { return sheet.set("media", m); }
  Node ownerNode() const { return {sheet["ownerNode"]}; }
  StyleSheet parentStyleSheet() const { return {sheet["parentStyleSheet"]}; }
  DOMString title() const { return sheet["title"].as<DOMString>(); }
  DOMString type() const { return sheet["type"].as<DOMString>(); }
};

#endif
