#ifndef C3_SELECT_SELECT_HPP
#define C3_SELECT_SELECT_HPP

#include "c3-select-data.hpp"

#include <experimental/type_traits>
#include <memory>
#include <variant>

using std::experimental::is_detected_v;

struct Select {
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
};

#endif
