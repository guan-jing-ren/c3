#ifndef C3_SELECT_SELECT_HPP
#define C3_SELECT_SELECT_HPP

#include "c3-select-fwd.hpp"

#include <experimental/type_traits>
#include <memory>
#include <set>
#include <variant>

using std::experimental::is_detected_v;

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

struct Select {
  static set<shared_ptr<void>, data_compare> datastore;

  unique_ptr<Select> group;

  using criteria_type = variant<string, string, Node, vector<Node>>;
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
    } else if constexpr (is_constructible_v<Node, C>) {
      if (!all)
        criteria = forward<C>(c);
      else
        criteria.emplace<3>({forward<C>(c)});
    } else if constexpr (is_constructible_v<vector<Node>, C>) {
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

  template <typename Database, typename KeyFunction>
  auto data(Database &&d, KeyFunction &&k) const {
    using database_type = view_type<decltype(d)>;
    using keyfunction_type = view_type<decltype(k)>;

    return Data<database_type, keyfunction_type>{*this, forward<Database>(d),
                                                 forward<KeyFunction>(k)};
  }

  template <typename Database> auto data(Database &&d) const {
    return data(forward<Database>(d), [](auto &value) {
      if constexpr (is_detected_v<key_type, Database>)
        return value.first;
      else
        return value;
    });
  }

  template <typename Database> Database &data() const {
    Database *db = nullptr;
    group->forEach([&db](Node &n) {
      void *address = nullptr;
      Element e{n.node};
      if (!e.hasAttribute("data-c3")) {
        address = datastore.emplace(shared_ptr<void>{make_shared<Database>()})
                      .first->get();
        e.setAttribute("data-c3", to_string(reinterpret_cast<size_t>(address)));
      } else
        address = reinterpret_cast<void *>(stoull(e.getAttribute("data-c3")));

      db = static_pointer_cast<Database>(*datastore.find(address)).get();
    });
    return *db;
  }

  struct lambda_view {
    template <typename C> static void caller(void *context, Node &n) {
      (*static_cast<C *>(context))(n);
    }

    void *context;
    void (*call)(void *, Node &);

    template <typename C>
    lambda_view(C &&c) : context(&c), call(&caller<decay_t<C>>) {}

    void operator()(Node &n) { call(context, n); }
  };

  template <typename F> Select &forEach(F &&f) {
    switch (criteria.index()) {
    case 0:
      if (group)
        group->forEach(lambda_view([this, &f](Node &n) mutable {
          auto query = ParentNode(n.node).querySelector(get<0>(criteria));
          f(query);
        }));
      else {
        auto query =
            Document{val::global("document")}.querySelector(get<0>(criteria));
        f(query);
      }
      break;
    case 1:
      if (group)
        group->forEach(lambda_view([this, &f](Node &n) mutable {
          for (auto &&query :
               ParentNode(n.node).querySelectorAll(get<1>(criteria)))
            f(query);
        }));
      else
        for (auto &&query : Document{val::global("document")}.querySelectorAll(
                 get<1>(criteria)))
          f(query);
      break;
    case 2:
      f(get<2>(criteria));
      break;
    case 3:
      for (auto &&node : get<3>(criteria))
        f(node);
      break;
    }
    return *this;
  }
};

set<shared_ptr<void>, data_compare> Select::datastore;
#endif
