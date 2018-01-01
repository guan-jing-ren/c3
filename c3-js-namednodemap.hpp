#ifndef C3_JS_NAMEDNODEMAP_HPP
#define C3_JS_NAMEDNODEMAP_HPP

#include "c3-js-attr.hpp"

struct NamedNodeMap {
  val map;
  unsigned long length() const { return map["length"].as<unsigned long>(); }
  Attr item(unsigned long index) const {
    return {map.call<val>("item", val(index))};
  }
  Attr getNamedItem(const DOMString &name) const {
    return {map.call<val>("getNamedItem", val(name))};
  }
  Attr getNamedItemNS(const DOMString &nameSpace,
                      const DOMString &localName) const {
    return {map.call<val>("getNamedItemNS", val(nameSpace), val(localName))};
  }
  Attr setNamedItem(Attr attr) const {
    return {map.call<val>("setNamedItem", attr.attr)};
  }
  Attr setNamedItemNS(Attr attr) const {
    return {map.call<val>("setNamedItemNS", attr.attr)};
  }
  Attr removeNamedItem(const DOMString &name) const {
    return {map.call<val>("removeNamedItem", val(name))};
  }
  Attr removeNamedItemNS(const DOMString &nameSpace,
                         const DOMString &localName) const {
    return {map.call<val>("removeNamedItemNS", val(nameSpace), val(localName))};
  }
};

#endif
