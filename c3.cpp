
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
    cout << "Child: " << child.nodeName() << ": " << child.textContent()
         << "\n";

  c3().selectAll("p").each([](val d, auto &n, auto &&v) {
    cout << "For each: " << n.nodeName() << ": " << n.textContent() << "\n";
    return d;
  });

  {
    val dat = val::array();
    val obj[2] = {val::object(), val::object()};
    obj[0].set("hello", 4);
    obj[1].set("world", 7);
    dat.call<void>("push", obj[0]);
    dat.call<void>("push", obj[1]);
    console_log(dat);
    auto data_sel = c3().selectAll("p").data(dat, [](val d, auto &, auto &&) {
      return val::global("JSON").call<val>("stringify", d);
    });
    data_sel.enter()
        .each([](val d, auto &, auto &&) {
          cout << "Enter for each:\n";
          console_log(d);
          return d;
        })
        .append([](val d, auto &, auto &&) { return val{"p"}; })
        .text([](val d, auto &, auto &&) {
          return val::global("JSON").call<val>("stringify", d);
        });
    data_sel.update().each([](val d, auto &, auto &&) {
      cout << "Update for each:\n";
      console_log(d);
      return d;
    });
    data_sel.exit().each([](val d, auto &, auto &&) {
      cout << "Exit for each:\n";
      console_log(d);
      return d;
    });
  }
  {
    val dat = val::array();
    val obj[2] = {val::object(), val::object()};
    obj[0].set("hello", 4);
    obj[1].set("world", 7);
    dat.call<void>("push", obj[0]);
    dat.call<void>("push", obj[1]);
    console_log(dat);
    auto data_sel = c3().selectAll("p").data(dat, [](val d, auto &, auto &&) {
      return val::global("JSON").call<val>("stringify", d);
    });
    data_sel.enter()
        .each([](val d, auto &, auto &&) {
          cout << "Enter for each:\n";
          console_log(d);
          return d;
        })
        .append([](val d, auto &, auto &&) { return val{"p"}; })
        .text([](val d, auto &, auto &&) {
          return val::global("JSON").call<val>("stringify", d);
        });
    data_sel.update().each([](val d, auto &, auto &&) {
      cout << "Update for each:\n";
      console_log(d);
      return d;
    });
    data_sel.exit().each([](val d, auto &, auto &&) {
      cout << "Exit for each:\n";
      console_log(d);
      return d;
    });
  }
  {
    val dat = val::array();
    console_log(dat);
    auto data_sel = c3().selectAll("p").data(dat, [](val d, auto &, auto &&) {
      return val::global("JSON").call<val>("stringify", d);
    });
    data_sel.enter()
        .each([](val d, auto &, auto &&) {
          cout << "Enter for each:\n";
          console_log(d);
          return d;
        })
        .append([](val d, auto &, auto &&) { return val{"p"}; })
        .text([](val d, auto &, auto &&) {
          return val::global("JSON").call<val>("stringify", d);
        });
    data_sel.update().each([](val d, auto &, auto &&) {
      cout << "Update for each:\n";
      console_log(d);
      return d;
    });
    data_sel.exit().each([](val d, auto &, auto &&) {
      cout << "Exit for each:\n";
      console_log(d);
      return d;
    });
  }
  // {
  //   vector<string> dat{{{"hello"}, {"world"}}};
  //   c3().select("*").data(dat).enter();
  // }
  // {
  //   HTMLCollection col;
  //   Node named = col.namedItem("name");
  //   for (auto node : col)
  //     ;
  //   vector<Node> vcol = col;
  // }
  // {
  //   NodeList list;
  //   list.each([](Node n) {});
  // }
  // {
  //   DOMTokenList list;
  //   list.each([](Node n) {});
  // }

  return 0;
}
