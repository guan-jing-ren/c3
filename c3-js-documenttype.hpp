#ifndef C3_JS_DOCUMENTTYPE_HPP
#define C3_JS_DOCUMENTTYPE_HPP

#include "c3-js-node.hpp"

struct DocumentType : virtual Node {
  using Node::Node;
  DOMString name() const { return node["name"].as<DOMString>(); }
  DOMString publicId() const { return node["publicId"].as<DOMString>(); }
  DOMString systemId() const { return node["systemId"].as<DOMString>(); }
};

#endif
