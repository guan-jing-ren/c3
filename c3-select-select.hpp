#ifndef C3_SELECT_SELECT_HPP
#define C3_SELECT_SELECT_HPP

#include "c3-select-fwd.hpp"

#include <experimental/type_traits>
#include <functional>
#include <memory>
#include <set>
#include <variant>

struct c3_factory {
  variant<std::function<val(val d, Element &, const vector<Element> &)>,
          std::function<val(val d, Element &)>, //
          std::function<val(val d)>,            //
          std::function<val()>,                 //
          val>
      f;

  template <typename F> c3_factory(F ff) : f(move(ff)) {}
  c3_factory(const char *v) : f(val{v}) {}
  c3_factory(double v) : f(val{v}) {}
  c3_factory(bool v) : f(val{v}) {}
  c3_factory(const DOMString &v) : f(val{v}) {}
  c3_factory(val v) : f(v) {}
  c3_factory(const c3_factory &) = default;
  c3_factory(c3_factory &&) = default;
  c3_factory &operator=(const c3_factory &) = default;
  c3_factory &operator=(c3_factory &&) = default;

  val operator()(val d, Element &e, const vector<Element> &v) const {
    switch (f.index()) {
    case 0:
      return get<0>(f)(d, e, v);
    case 1:
      return get<1>(f)(d, e);
    case 2:
      return get<2>(f)(d);
    case 3:
      return get<3>(f)();
    case 4:
      return get<4>(f);
    }
    return val::null();
  }
};

inline c3_factory c3_identity = [](val d) { return d; };

struct Select {
  unique_ptr<Select> group;

  using criteria_type =
      variant<string, string, Element, vector<Element>,
              std::function<void(c3_factory &)>, vector<Select>>;
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

  template <typename C> Select select(C &&c, bool all) const {
    return {*this, forward<C>(c), all};
  }

  template <typename C> Select select(C &&c) const {
    return select(forward<C>(c), false);
  }

  template <typename C> Select selectAll(C &&c) const {
    return select(forward<C>(c), true);
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

  vector<Element> nodes() {
    vector<Element> ns;
    each([&ns](val v, auto &e) mutable {
      ns.push_back(e);
      return v;
    });
    return ns;
  }

  Select &each(c3_factory f) {
    auto applicator = [&f](Element &n, const vector<Element> &nodes) mutable {
      f(n.node["data-c3"], n, nodes);
    };

    switch (criteria.index()) {
    case 0:
      if (group) {
        for (auto &&n : group->nodes()) {
          auto query = ParentNode(n.node).querySelector(get<0>(criteria));
          applicator(query, {{query}});
        }
      } else {
        auto query = Document{}.querySelector(get<0>(criteria));
        applicator(query, {{query}});
      }
      break;
    case 1:
      if (group)
        for (auto &&n : group->nodes()) {
          auto qs = ParentNode(n.node).querySelectorAll(get<1>(criteria));
          vector<Element> queries{begin(qs), end(qs)};
          for (auto &&query : queries)
            applicator(query, queries);
        }
      else {
        auto qs = Document{}.querySelectorAll(get<1>(criteria));
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
      get<3>(append_sel.criteria).push_back(element);
      return d;
    });
    return append_sel;
  }

  Select insert(c3_factory type, c3_factory before) {
    Select insert_sel{*this};
    insert_sel.criteria.emplace<3>(vector<Element>{});
    each([&insert_sel, &type, &before](val d, auto &e, auto &&v) mutable {
      Element element = e.insertBefore(
          e.ownerDocument()
              .createElement(type(d, e, v).template as<DOMString>())
              .node,
          e.querySelector(before(d, e, v).template as<DOMString>()).node);
      if (!d.isNull())
        element.node.set("data-c3", d);
      get<3>(insert_sel.criteria).push_back(element);
      return d;
    });
    return insert_sel;
  }

  Select &remove() {
    each([](val d, auto &e) {
      e.parentNode().removeChild(e.node);
      return d;
    });
    return *this;
  }

  Select &classed(const DOMString &classes, c3_factory f) {
    each([&classes, &f](val d, auto &&e, auto &&v) mutable {
      if (f(d, e, v).template as<bool>())
        e.classList().add(classes);
      else
        e.classList().remove(classes);
      return d;
    });
    return *this;
  }

  Select &style(const DOMString &property, c3_factory f) {
    each([&property, &f](val d, auto &e, auto &&v) mutable {
      e.style().setProperty(property, f(d, e, v).template as<DOMString>());
      return d;
    });
    return *this;
  }

  Select &attr(const DOMString &key, c3_factory value) {
    each([&key, &value](val d, auto &e, auto &&v) mutable {
      e.setAttribute(key, value(d, e, v));
      return d;
    });
    return *this;
  }

  Select &property(const DOMString &key, c3_factory value) {
    each([&key, &value](val d, auto &e, auto &&v) mutable {
      e.node.set(key, value(d, e, v));
      return d;
    });
    return *this;
  }

  Select &text(c3_factory text) {
    each([&text](val d, auto &e, auto &&v) mutable {
      e.textContent(text(d, e, v).template as<string>());
      return d;
    });
    return *this;
  }

  Select &html(c3_factory html) {
    each([&html](val d, auto &e, auto &&v) mutable {
      e.innerHTML(html(d, e, v).template as<string>());
      return d;
    });
    return *this;
  }
};

#endif
