#ifndef C3_SELECT_SELECT_HPP
#define C3_SELECT_SELECT_HPP

#include "c3-select-fwd.hpp"

#include <experimental/type_traits>
#include <memory>
#include <set>
#include <variant>

using std::experimental::is_detected_v;

template <typename F, typename... Args>
using callable = decltype(declval<F &&>()(declval<Args &&>()...));

struct data_compare {
  using is_transparent = void *;
  bool operator()(const shared_ptr<void> &sp, const void *p) const {
    return sp.get() < p;
  }
  bool operator()(const void *p, const shared_ptr<void> &sp) const {
    return p < sp.get();
  }
  bool operator()(const shared_ptr<void> &l, const shared_ptr<void> &r) const {
    return l < r;
  }
};

template <typename F> struct Wrappable { using data_type = nullptr_t; };
template <typename R> struct Wrappable<R()> {
  using data_type = nullptr_t;
  using result_type = R;
};
template <typename R, typename Data>
struct Wrappable<R(Data)> : Wrappable<R()> {
  using data_type = Data;
};
template <typename R> struct Wrappable<R(Element)> : Wrappable<R()> {};
template <typename R, typename Data>
struct Wrappable<R(Data, Element)> : Wrappable<R(Data)> {};
template <typename R, typename Data>
struct Wrappable<R(Element, Data)> : Wrappable<R(Data)> {};
template <typename R, typename Data>
struct Wrappable<R(Data, Element, vector<Element>)> : Wrappable<R(Data)> {};
template <typename R, typename Data>
struct Wrappable<R(Data, vector<Element>, Element)> : Wrappable<R(Data)> {};
template <typename R, typename Data>
struct Wrappable<R(Element, Data, vector<Element>)> : Wrappable<R(Data)> {};
template <typename R, typename Data>
struct Wrappable<R(Element, vector<Element>, Data)> : Wrappable<R(Data)> {};
template <typename R, typename Data>
struct Wrappable<R(vector<Element>, Data, Element)> : Wrappable<R(Data)> {};
template <typename R, typename Data>
struct Wrappable<R(vector<Element>, Element, Data)> : Wrappable<R(Data)> {};
template <typename R, typename... Args>
struct Wrappable<R (*)(Args...)> : Wrappable<R(decay_t<Args>...)> {};
template <typename R, typename F, typename... Args>
struct Wrappable<R (F::*)(Args...)> : Wrappable<R(decay_t<Args>...)> {};

template <typename T, typename D = decay_t<T>>
using deduce_data = typename Wrappable<
    conditional_t<is_class_v<D>, decltype(&D::operator()), D>>::data_type;
template <typename T, typename D = decay_t<T>>
using deduce_result = typename Wrappable<
    conditional_t<is_class_v<D>, decltype(&D::operator()), D>>::result_type;

template <typename F, typename Data = deduce_data<F>> auto make_wrapper(F &&f) {
  if constexpr (is_detected_v<callable, F, Data &>)
    return [f = forward<F>(f)](
               Data & d, Element & n,
               const vector<Element> &v) mutable->decltype(auto) {
      return f(d);
    };
  else if constexpr (is_detected_v<callable, F, Element &>)
    return [f = forward<F>(f)](
               Data & d, Element & n,
               const vector<Element> &v) mutable->decltype(auto) {
      return f(n);
    };
  else if constexpr (is_detected_v<callable, F, Data &, Element &>)
    return [f = forward<F>(f)](
               Data & d, Element & n,
               const vector<Element> &v) mutable->decltype(auto) {
      return f(d, n);
    };
  else if constexpr (is_detected_v<callable, F, Element &, Data &>)
    return [f = forward<F>(f)](
               Data & d, Element & n,
               const vector<Element> &v) mutable->decltype(auto) {
      return f(n, d);
    };
  else if constexpr (is_detected_v<callable, F, Data &, Element &,
                                   const vector<Element> &>)
    return forward<F>(f);
  else if constexpr (is_detected_v<callable, F, Data &, const vector<Element> &,
                                   Element &>)
    return [f = forward<F>(f)](
               Data & d, Element & n,
               const vector<Element> &v) mutable->decltype(auto) {
      return f(d, v, n);
    };
  else if constexpr (is_detected_v<callable, F, Element &, Data &,
                                   const vector<Element> &>)
    return [f = forward<F>(f)](
               Data & d, Element & n,
               const vector<Element> &v) mutable->decltype(auto) {
      return f(n, d, v);
    };
  else if constexpr (is_detected_v<callable, F, Element &,
                                   const vector<Element> &, Data &>)
    return [f = forward<F>(f)](
               Data & d, Element & n,
               const vector<Element> &v) mutable->decltype(auto) {
      return f(n, v, d);
    };
  else if constexpr (is_detected_v<callable, F, const vector<Element> &, Data &,
                                   Element &>)
    return [f = forward<F>(f)](
               Data & d, Element & n,
               const vector<Element> &v) mutable->decltype(auto) {
      return f(v, d, n);
    };
  else if constexpr (is_detected_v<callable, F, const vector<Element> &,
                                   Element &, Data &>)
    return [f = forward<F>(f)](
               Data & d, Element & n,
               const vector<Element> &v) mutable->decltype(auto) {
      return f(v, n, d);
    };
  else if constexpr (is_detected_v<callable, F>)
    return [f = forward<F>(f)](
               Data & d, Element & n,
               const vector<Element> &v) mutable->decltype(auto) {
      return f();
    };
}

template <typename Data, typename F>
using wrappable = enable_if_t<
    is_detected_v<callable, F, Data &> ||
    is_detected_v<callable, F, Element &> ||
    is_detected_v<callable, F, Data &, Element &> ||
    is_detected_v<callable, F, Element &, Data &> ||
    is_detected_v<callable, F, Data &, Element &, const vector<Element> &> ||
    is_detected_v<callable, F, Data &, const vector<Element> &, Element &> ||
    is_detected_v<callable, F, Element &, Data &, const vector<Element> &> ||
    is_detected_v<callable, F, Element &, const vector<Element> &, Data &> ||
    is_detected_v<callable, F, const vector<Element> &, Data &, Element &> ||
    is_detected_v<callable, F, const vector<Element> &, Element &, Data &> ||
    is_detected_v<callable, F>>;

struct Select {
  static set<shared_ptr<void>, data_compare> datastore;

  unique_ptr<Select> group;

  using criteria_type = variant<string, string, Element, vector<Element>>;
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

  template <typename Datafunc, typename KeyFunction,
            typename Database = deduce_result<Datafunc>,
            typename Datum = deduce_data<Datafunc>,
            typename = enable_if_t<is_detected_v<wrappable, Datum, Datafunc>>>
  auto data(Datafunc &&df, KeyFunction &&kf) {
    Database database;
    forEach([ df = make_wrapper(forward<Datafunc>(df)),
              &database ](Datum & d, Element & n,
                          const vector<Element> &v) mutable { df(d, n, v); });
    return Data<Database, KeyFunction>{*this, move(database),
                                       forward<KeyFunction>(kf)};
  }

  template <typename T> using first_element = tuple_element_t<0, T>;

  template <typename Datafunc, typename Database = deduce_result<Datafunc>,
            typename Datum = deduce_data<Datafunc>,
            typename = enable_if_t<is_detected_v<wrappable, Datum, Datafunc>>>
  auto data(Datafunc &&df) {
    return data(
        forward<Datafunc>(df),
        [](decltype(*begin(declval<Database>())) &value) -> decltype(auto) {
          if constexpr (is_detected_v<first_element, decay_t<decltype(value)>>)
            return get<0>(value);
          else
            return value;
        });
  }

  template <
      typename Database, typename KeyFunction,
      typename = enable_if_t<!is_detected_v<wrappable, nullptr_t, Database>>>
  auto data(Database &&d, KeyFunction &&k) {
    return data([d = forward<Database>(d)]() mutable->decltype(
                    auto) { return d; },
                forward<KeyFunction>(k));
  }

  template <
      typename Database,
      typename = enable_if_t<!is_detected_v<wrappable, nullptr_t, Database>>>
  auto data(Database &&d) {
    return data([d = forward<Database>(d)]() mutable->decltype(auto) {
      return d;
    });
  }

  template <typename Database> Database &data() const {
    Database *db = nullptr;
    auto retriever = [&db](Element &e) mutable {
      void *address = nullptr;
      if (!e.hasAttribute("data-c3") ||
          (e.getAttribute("data-c3-typeinfo-name") !=
           typeid(remove_const_t<remove_reference_t<Database>>).name())) {
        address = datastore.emplace(shared_ptr<void>{make_shared<Database>()})
                      .first->get();
        e.setAttribute("data-c3", to_string(reinterpret_cast<size_t>(address)));
        e.setAttribute(
            "data-c3-typeinfo-name",
            typeid(remove_const_t<remove_reference_t<Database>>).name());

      } else
        address = reinterpret_cast<void *>(stoull(e.getAttribute("data-c3")));

      db = static_pointer_cast<Database>(*datastore.find(address)).get();
    };

    if (group)
      group->forEach(retriever);
    else {
      Element doc = Document{val::global("document")}.documentElement();
      retriever(doc);
    }
    return *db;
  }

  struct lambda_view {
    template <typename C> static void caller(void *context, Element &n) {
      (*static_cast<C *>(context))(n);
    }

    void *context;
    void (*call)(void *, Element &);

    template <typename C>
    lambda_view(C &&c) : context(&c), call(&caller<decay_t<C>>) {}

    void operator()(Element &n) { call(context, n); }
  };

  vector<Element> nodes() {
    vector<Element> ns;
    forEach([&ns](Element &e) mutable { ns.push_back(e); });
    return ns;
  }

  template <typename F, typename Datum = deduce_data<F>>
  Select &forEach(F &&f) {
    auto f_wrapped = make_wrapper(forward<F>(f));
    Datum datum;
    auto applicator = [&datum, &f_wrapped](
                          Element &n, const vector<Element> &nodes) mutable {
      if (n.hasAttribute("data-c3"))
        f_wrapped(*reinterpret_cast<Datum *>(stoull(n.getAttribute("data-c3"))),
                  n, nodes);
      else
        f_wrapped(datum, n, nodes);
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
    }

    return *this;
  }
};

set<shared_ptr<void>, data_compare> Select::datastore;
#endif
