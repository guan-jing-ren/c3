#ifndef C3_JS_ATTR_HPP
#define C3_JS_ATTR_HPP

#include "c3-js-fwd.hpp"

struct Attr {
  val attr;

  DOMString name() const { return attr["name"].as<DOMString>(); }
  DOMString namespaceURI() const {
    return attr["namespaceURI"].as<DOMString>();
  }
  DOMString localName() const { return attr["localName"].as<DOMString>(); }
  template <typename E = Element> E ownerElement() const {
    return {attr["ownerElement"]};
  }
  bool specified() const { return attr["specified"].as<bool>(); }
  DOMString value() const { return attr["value"].as<DOMString>(); }
  void value(const DOMString &v) { return attr.set("value", v); }
};

#endif
