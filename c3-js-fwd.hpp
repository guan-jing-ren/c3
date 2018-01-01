#ifndef C3_JS_FWD_HPP
#define C3_JS_FWD_HPP

#include <emscripten.h>
#include <emscripten/bind.h>
#include <emscripten/val.h>

#include <string>

struct Node;
struct Parent;
struct Attribute;
struct Element;
struct Document;
struct Window;
struct Screen;
struct Navigator;
struct Location;

using DOMString = string;

template <typename T = val> struct Iterator;

struct NodeList;
struct DOMTokenList;
struct HTMLCollection;

#endif
