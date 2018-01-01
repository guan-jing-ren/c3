#ifndef C3_JS_DOMIMPLEMENTATION_HPP
#define C3_JS_DOMIMPLEMENTATION_HPP

#include "c3-js-documenttype.hpp"

struct DOMImplementation {
  val impl = val::null();

  template <typename D = Document>
  D createDocument(const DOMString &nameSpace, const DOMString &name,
                   optional<DocumentType> doctype = nullopt) {
    if (doctype)
      return {impl.call<val>("createDocument", val(nameSpace), val(name),
                             doctype->node)};
    else
      return {impl.call<val>("createDocument", val(nameSpace), val(name))};
  }
  DocumentType createDocumentType(const DOMString &name,
                                  const DOMString &publicId,
                                  const DOMString &systemId) const {
    return {impl.call<val>("createDocumentType", val(name), val(publicId),
                           val(systemId))};
  }
  template <typename D = Document>
  D createHTMLDocument(const DOMString &title) const {
    return {impl.call<val>("createHTMLDocument", val(title))};
  }
};

#endif
