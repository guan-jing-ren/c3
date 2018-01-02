
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

  c3().select("body").selectAll("p").forEach([](const Node &n) {
    cout << n.nodeName() << ": " << n.textContent() << "\n";
  });

  {
    map<string, int> dat{{{"hello", 4}, {"world", 7}}};
    c3().select("*").data(dat).enter();
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
