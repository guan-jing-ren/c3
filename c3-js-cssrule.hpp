#ifndef C3_JS_CSSRULE_HPP
#define C3_JS_CSSRULE_HPP

#include "c3-js-cssstylesheet.hpp"

struct CSSRule {
  val rule;

  DOMString cssText() const { return rule["cssText"].as<DOMString>(); }

  void cssText(const DOMString &css) { rule.set("cssText", val{css}); }

  CSSRule parentRule() const { return {rule["parentRule"]}; }

  CSSStyleSheet parentStylesheet() const { return {rule["parentStylesheet"]}; }

  unsigned type() const { return rule["type"].as<unsigned>(); }
};

#endif
