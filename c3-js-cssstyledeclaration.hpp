#ifndef C3_JS_CSSSTYLEDECLARATION_HPP
#define C3_JS_CSSSTYLEDECLARATION_HPP

#include "c3-js-cssrule.hpp"

struct CSSStyleDeclaration {
  val decl;

  DOMString cssText() const { return decl["cssText"].as<DOMString>(); }

  void cssText(const DOMString &css) { decl.set("cssText", val{css}); }

  unsigned length() const { return decl["length"].as<unsigned>(); }

  CSSRule parentRule() const { return {decl["parentRule"]}; }

  DOMString getPropertyPriority(const DOMString &property) const {
    return decl.call<DOMString>("getPropertyPriority", val{property});
  }

  DOMString getPropertyValue(const DOMString &property) const {
    return decl.call<DOMString>("getPropertyValue", val{property});
  }

  DOMString item(unsigned index) const {
    return decl.call<DOMString>("item", val{index});
  }

  DOMString removeProperty(const DOMString &property) const {
    return decl.call<DOMString>("removeProperty", val{property});
  }

  void setProperty(const DOMString &property, const DOMString &value = "",
                   const optional<DOMString> &priority = {}) const {
    if (priority)
      decl.call<void>("setProperty", val{property}, val{value},
                      priority.value());
    else
      decl.call<void>("setProperty", val{property}, val{value});
  }
};

#endif
