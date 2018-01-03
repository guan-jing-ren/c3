#ifndef C3_SELECT_DATA_HPP
#define C3_SELECT_DATA_HPP

#include "c3-select-select.hpp"

template <typename Database, typename KeyFunction> struct Data {
  unique_ptr<Select> group;

  Database database;
  KeyFunction keyfunction;

  Data(const Select &g, Database d, KeyFunction k)
      : group(make_unique<Select>(g)), database(d), keyfunction(k) {}

  template <typename S = Select> S enter() {
    cout << size(database) << "\n";
    for (auto &data : database)
      cout << keyfunction(data) << "\n";

    Database &data = group->data<remove_reference_t<Database>>();

    return {*group};
  }
  template <typename S = Select> S exit() { return {*group}; }
};

#endif
