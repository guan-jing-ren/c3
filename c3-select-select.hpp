#ifndef C3_SELECT_SELECT_HPP
#define C3_SELECT_SELECT_HPP

#include "c3-select-fwd.hpp"

#include <experimental/type_traits>
#include <functional>
#include <memory>
#include <set>
#include <variant>

using c3_factory = std::function<val(val, Element &, const vector<Element> &)>;
inline c3_factory c3_identity = [](val d, auto &, auto &&) { return d; };

struct Select {
  unique_ptr<Select> group;

  using criteria_type =
      variant<string, string, Element, vector<Element>,
              std::function<void(c3_factory &)>, std::vector<Select>>;
  criteria_type criteria;

  Select(const Select &s)
      : group(s.group ? make_unique<Select>(*s.group) : nullptr),
        criteria(s.criteria) {}
  template <typename C> Select(C &&c, bool all) {
    if constexpr (is_constructible_v<DOMString, C>) {
      if (!all)
        criteria.emplace<0>(forward<C>(c));
      else
        criteria.emplace<1>(forward<C>(c));
    } else if constexpr (is_constructible_v<Element, C>) {
      if (!all)
        criteria = forward<C>(c);
      else
        criteria.emplace<3>({forward<C>(c)});
    } else if constexpr (is_constructible_v<vector<Element>, C>) {
      if (!all)
        criteria = c[0];
      else
        criteria = forward<C>(c);
    }
  }
  template <typename C>
  Select(Select s, C &&c, bool all) : Select(forward<C>(c), all) {
    group = make_unique<Select>(s);
  }

  template <typename C> Select select(C &&c) const {
    return {*this, forward<C>(c), false};
  }
  template <typename C> Select selectAll(C &&c) const {
    return {*this, forward<C>(c), true};
  }

  template <typename D = Data> D data(c3_factory df, c3_factory kf) {
    return D{*this, move(df), move(kf)};
  }

  template <typename D = Data> D data(c3_factory df) {
    return data<D>(move(df), c3_identity);
  }

  template <typename D = Data> D data(val d, c3_factory kf) {
    return D{*this, d, move(kf)};
  }

  template <typename D = Data> D data(val d) { return data<D>(d, c3_identity); }

  // template <typename Database> Database &data() const {}

  vector<Element> nodes() {
    vector<Element> ns;
    each([&ns](val v, auto &e, auto &&) mutable {
      ns.push_back(e);
      return v;
    });
    return ns;
  }

  Select &each(c3_factory f) {
    auto applicator = [f](Element &n, const vector<Element> &nodes) mutable {
      f(n.node["data-c3"], n, nodes);
    };

    switch (criteria.index()) {
    case 0:
      if (group) {
        for (Element &n : group->nodes()) {
          auto query = ParentNode(n.node).querySelector(get<0>(criteria));
          applicator(query, {{query}});
        }
      } else {
        auto query =
            Document{val::global("document")}.querySelector(get<0>(criteria));
        applicator(query, {{query}});
      }
      break;
    case 1:
      if (group)
        for (Element &n : group->nodes()) {
          auto qs = ParentNode(n.node).querySelectorAll(get<1>(criteria));
          vector<Element> queries{begin(qs), end(qs)};
          for (auto &&query : queries)
            applicator(query, queries);
        }
      else {
        auto qs = Document{val::global("document")}.querySelectorAll(
            get<1>(criteria));
        vector<Element> queries{begin(qs), end(qs)};
        for (auto &&query : queries)
          applicator(query, queries);
      }
      break;
    case 2:
      applicator(get<2>(criteria), {{get<2>(criteria)}});
      break;
    case 3:
      for (auto &&n : get<3>(criteria))
        applicator(n, get<3>(criteria));
      break;
    case 4:
      get<4>(criteria)(f);
      break;
    case 5:
      for (auto &sel : get<5>(criteria))
        sel.each(f);
      break;
    }
    return *this;
  }

  Select append(c3_factory f) {
    Select append_sel{*this};
    append_sel.criteria.emplace<3>(vector<Element>{});
    each([&append_sel, &f](val d, auto &e, auto &&v) mutable {
      auto tag = f(d, e, v).template as<DOMString>();
      Element element =
          e.appendChild(e.ownerDocument().createElement(tag).node);
      if (!d.isNull())
        element.node.set("data-c3", d);
      // e.appendChild(element.node);
      get<3>(append_sel.criteria).push_back(element);
      return d;
    });
    return append_sel;
  }
  Select insert(c3_factory f) { return *this; }
  Select remove(c3_factory f) { return *this; }
  Select &classed(c3_factory f) { return *this; }
  Select &style(c3_factory f) { return *this; }
  Select &attr(c3_factory key, c3_factory value) { return *this; }
  Select &property(c3_factory key, c3_factory value) { return *this; }
  Select &text(c3_factory text) {
    each([&text](val d, auto &e, auto &&v) {
      e.textContent(text(d, e, v).template as<string>());
      return d;
    });
    return *this;
  }
  Select &html(c3_factory html) { return *this; }
};

#endif
