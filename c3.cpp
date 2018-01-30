
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
    auto data_sel = c3().selectAll("p").data(dat, [](val d) {
      return val::global("JSON").call<val>("stringify", d);
    });
    data_sel.enter()
        .each([](val d) {
          cout << "Enter for each:\n";
          console_log(d);
          return d;
        })
        .append([](val d) { return val{"p"}; })
        .text([](val d) {
          return val::global("JSON").call<val>("stringify", d);
        });
    data_sel.update().each([](val d) {
      cout << "Update for each:\n";
      console_log(d);
      return d;
    });
    data_sel.exit().each([](val d) {
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
    auto data_sel = c3().selectAll("p").data(dat, [](val d) {
      return val::global("JSON").call<val>("stringify", d);
    });
    data_sel.enter()
        .each([](val d) {
          cout << "Enter for each:\n";
          console_log(d);
          return d;
        })
        .append([](val d) { return val{"p"}; })
        .text([](val d) {
          return val::global("JSON").call<val>("stringify", d);
        });
    data_sel.update().each([](val d) {
      cout << "Update for each:\n";
      console_log(d);
      return d;
    });
    data_sel.exit().each([](val d) {
      cout << "Exit for each:\n";
      console_log(d);
      return d;
    });
  }
  {
    val dat = val::array();
    console_log(dat);
    auto data_sel = c3().selectAll("p").data(dat, [](val d) {
      return val::global("JSON").call<val>("stringify", d);
    });
    data_sel.enter()
        .each([](val d) {
          cout << "Enter for each:\n";
          console_log(d);
          return d;
        })
        .append([](val d) { return val{"p"}; })
        .text([](val d) {
          return val::global("JSON").call<val>("stringify", d);
        });
    data_sel.update().each([](val d) {
      cout << "Update for each:\n";
      console_log(d);
      return d;
    });
    data_sel.exit().each([](val d) {
      cout << "Exit for each:\n";
      console_log(d);
      return d;
    });
    data_sel.exit().remove();
  }
  {
    auto table_sel =
        c3().select("body").append([](auto) { return val{"table"}; });
    int array[3][4][5][6] = {}, count = 0;
    val array_val = val::array();
    for (auto &&i : array) {
      val array_val_i = val::array();
      array_val.call<void>("push", array_val_i);
      for (auto &&j : i) {
        val array_val_j = val::array();
        array_val_i.call<void>("push", array_val_j);
        for (auto &&k : j) {
          val array_val_k = val::array();
          array_val_j.call<void>("push", array_val_k);
          for (auto &&l : k) {
            l = ++count;
            array_val_k.call<void>("push", l);
          }
        }
      }
    }
    console_log(array_val);
    auto stringify = [](val d) {
      return val::global("JSON").call<val>("stringify", d);
    };
    table_sel.selectAll("tr")
        .data(array_val, stringify)
        .enter()
        .append("tr")
        .selectAll("td")
        .data(c3_identity, stringify)
        .enter()
        .append("td")
        .append("table")
        .selectAll("tr")
        .data(c3_identity, stringify)
        .enter()
        .append("tr")
        .selectAll("td")
        .data(c3_identity, stringify)
        .enter()
        .append("td")
        .text(stringify);
    table_sel.selectAll("tr")
        .data(array_val, stringify)
        .enter()
        .append("tr")
        .selectAll("td")
        .data(c3_identity, stringify)
        .enter()
        .append("td")
        .append("table")
        .selectAll("tr")
        .data(c3_identity, stringify)
        .enter()
        .append("tr")
        .selectAll("td")
        .data(c3_identity, stringify)
        .enter()
        .append("td")
        .text(stringify);
    // .data(c3_identity, stringify)
    // .enter();
  }
  // {
  //   vector<string> dat{{{"hello"}, {"world"}}};
  //   c3().select("*").data(dat).enter();
  // }
  // {
  //   HTMLCollection col;
  //   Node named = col.namedItem("name");
  //   for (auto &&node : col)
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
