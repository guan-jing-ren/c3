#ifndef C3_JS_TEXT_HPP
#define C3_JS_TEXT_HPP

#include "c3-js-characterdata.hpp"
#include "c3-js-node.hpp"

struct Text : virtual CharacterData {
  using CharacterData::CharacterData;

  DOMString wholeText() const { return node["wholeText"].as<DOMString>(); }

  Node splitText(unsigned offset) const {
    return {node.call<val>("splitText", val(offset))};
  }
};

#endif
