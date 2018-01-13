
#include <algorithm>
#include <functional>
#include <iostream>
#include <map>
#include <optional>
#include <vector>

namespace emscripten {}
using namespace emscripten;
using namespace std;

#include "c3-js-document.hpp"

#include "c3-select-fwd.hpp"

#include "c3-select-data.hpp"
#include "c3-select-select.hpp"

#include "c3.hpp"

struct Transition {};

void wrapped();
void wrapped0(int);
void wrapped1(Element &);
void wrapped2(int, Element &);
void wrapped3(Element &, int);
void wrapped4(int, const vector<Element> &, Element &);
void wrapped5(int, Element &, const vector<Element> &);
void wrapped6(Element &, int, const vector<Element> &);
void wrapped7(Element &, const vector<Element> &, int);
void wrapped8(const vector<Element> &, int, Element &);
void wrapped9(const vector<Element> &, Element &, int);
void unwrapped(string);

static_assert(is_detected_v<wrappable, int, decltype(wrapped)>);
static_assert(is_detected_v<wrappable, int, decltype(wrapped0)>);
static_assert(is_detected_v<wrappable, int, decltype(wrapped1)>);
static_assert(is_detected_v<wrappable, int, decltype(wrapped2)>);
static_assert(is_detected_v<wrappable, int, decltype(wrapped3)>);
static_assert(is_detected_v<wrappable, int, decltype(wrapped4)>);
static_assert(is_detected_v<wrappable, int, decltype(wrapped5)>);
static_assert(is_detected_v<wrappable, int, decltype(wrapped6)>);
static_assert(is_detected_v<wrappable, int, decltype(wrapped7)>);
static_assert(is_detected_v<wrappable, int, decltype(wrapped8)>);
static_assert(is_detected_v<wrappable, int, decltype(wrapped9)>);

int main() {
  clog << "c3 loaded.\n";

  Document doc{val::global("document")};
  Element para = doc.createElement("p");
  para.textContent("Hello World!");
  doc.body().appendChild(para);
  doc.body()
      .insertBefore(doc.createElement("p"), para)
      .textContent("Goodbye World!");
  for (auto &&child : doc.body().children())
    cout << child.nodeName() << ": " << child.textContent() << "\n";

  c3().selectAll("p").forEach([](const Element &n) {
    cout << n.nodeName() << ": " << n.textContent() << "\n";
  });

  {
    map<string, int> dat{{{"hello", 4}, {"world", 7}}};
    c3().select("p").data(dat).enter();
  }
  {
    vector<string> dat{{{"hello"}, {"world"}}};
    c3().select("*").data(dat).enter();
  }
  {
    HTMLCollection col;
    Node named = col.namedItem("name");
    for (auto node : col)
      ;
    vector<Node> vcol = col;
  }
  {
    NodeList list;
    list.forEach([](Node n) {});
  }
  {
    DOMTokenList list;
    list.forEach([](Node n) {});
  }

  return 0;
}
