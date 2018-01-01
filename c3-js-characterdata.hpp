#ifndef C3_JS_CHARACTERDATA_HPP
#define C3_JS_CHARACTERDATA_HPP

#include "c3-js-node.hpp"
#include "c3-js-nondocumenttypechildnode.hpp"

struct CharacterData : virtual Node, virtual NonDocumentTypeChildNode {
  using Node::Node;

  DOMString data() const { return node["data"].as<DOMString>(); }
  unsigned long length() const { return node["length"].as<unsigned long>(); }

  DOMString substringData(unsigned long offset, unsigned long count) const {
    return node.call<DOMString>("substringData", val(offset), val(count));
  }
  void appendData(const DOMString &data) const {
    node.call<void>("appendData", val(data));
  }
  void insertData(unsigned long offset, const DOMString &data) const {
    node.call<void>("insertData", val(offset), val(data));
  }
  void deleteData(unsigned long offset, unsigned long count) const {
    node.call<void>("deleteData", val(offset), val(count));
  }
  void replaceData(unsigned long offset, unsigned long count,
                   const DOMString &data) const {
    node.call<void>("replaceData", val(count), val(data));
  }
};

#endif
