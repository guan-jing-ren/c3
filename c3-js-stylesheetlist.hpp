#ifndef C3_JS_STYLESHEETLIST_HPP
#define C3_JS_STYLESHEETLIST_HPP

#include "c3-js-cssstylesheet.hpp"

template <typename S = CSSStyleSheet>
struct StyleSheetList {
  val list = val::null();

   operator vector<S>() const {
    return vecFromJSArray<S>(list);
  }
};

#endif
