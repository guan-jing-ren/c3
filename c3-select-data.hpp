#ifndef C3_SELECT_DATA_HPP
#define C3_SELECT_DATA_HPP

#include "c3-select-select.hpp"

struct Data {
  Select selection;
  variant<val, c3_factory> datafunc;
  c3_factory keyfunc; // keyfunc(datafunc[i])
  vector<val> en, up, ex;

  std::function<bool(const val &l, const val &r)> bicomp =
      [ this, e = Element{}, v = vector<Element>{} ](auto &&l,
                                                     auto &&r) mutable {
    return keyfunc(l, e, v).template as<string>() <
           keyfunc(r, e, v).template as<string>();
  };

  Data(const Select &s, decltype(datafunc) df, c3_factory kf)
      : selection(s), datafunc(move(df)), keyfunc(move(kf)) {

    vector<val> data, predata;
    if (!selection.group)
      selection.group =
          make_unique<Select>(Document{}.documentElement(), false);
    switch (datafunc.index()) {
    case 0: {
      auto new_data = get<0>(datafunc);
      data = vecFromJSArray<val>(new_data);
      selection.group->nodes()[0].node.set("data-c3", new_data);
    } break;
    case 1:
      selection.group->each([ datafunc = get<1>(datafunc),
                              &data ](val d, auto &e, auto &&v) mutable {
        val new_val = datafunc(d, e, v);
        e.node.set("data-c3", new_val);
        auto new_data = vecFromJSArray<val>(new_val);
        data.insert(end(data), begin(new_data), end(new_data));
        return d;
      });
      break;
    }

    selection.each([&predata](val d) mutable {
      if (!d.isNull() && !d.isUndefined())
        predata.push_back(d);
      return d;
    });

    sort(begin(data), end(data), bicomp);
    sort(begin(predata), end(predata), bicomp);
    set_difference(begin(data), end(data), begin(predata), end(predata),
                   back_inserter(en), bicomp);

    set_difference(begin(predata), end(predata), begin(data), end(data),
                   back_inserter(ex), bicomp);

    set_intersection(begin(data), end(data), begin(predata), end(predata),
                     back_inserter(up), bicomp);
  }

  Select enter() {
    Select entered{selection};
    entered.criteria.emplace<4>(
        [ en = en, bicomp = bicomp,
          group = *selection.group ](c3_factory & f) mutable {
          group.each([&en, &bicomp, &f](val d, auto &e, auto &&v) mutable {
            if (d.isNull() || d.isUndefined())
              return d;
            auto new_data = vecFromJSArray<val>(d);
            for (auto &&n : new_data) {
              auto new_found = lower_bound(begin(en), end(en), n, bicomp);
              if (new_found != end(en))
                f(n, e, v);
            }
            return d;
          });
        });
    return entered;
  }

  Select update() {
    Select updated{selection};
    updated.criteria.emplace<5>(vector<Select>{});
    selection.each([this, &updated](val d, auto &e, auto &&v) mutable {
      if (d.isNull() || d.isUndefined())
        return d;
      auto new_val_i = lower_bound(begin(up), end(up), d, bicomp);
      if (new_val_i != cend(up)) {
        Select update_sel{selection};
        e.node.set("data-c3", *new_val_i);
        update_sel.criteria.emplace<4>(
            [e, v](c3_factory f) mutable { f(e.node["data-c3"], e, v); });
        get<5>(updated.criteria).push_back(update_sel);
      }
      return d;
    });

    return updated;
  }

  Select exit() {
    Select exited{selection};
    exited.criteria.emplace<5>(vector<Select>{});
    selection.each([this, &exited](val d, auto &e, auto &&v) mutable {
      if (d.isNull() || d.isUndefined())
        return d;
      auto new_val_i = lower_bound(begin(ex), end(ex), d, bicomp);
      if (new_val_i != cend(ex)) {
        Select exit_sel{selection};
        exit_sel.criteria.emplace<4>(
            [e, v](c3_factory f) mutable { f(e.node["data-c3"], e, v); });
        get<5>(exited.criteria).push_back(exit_sel);
      }
      return d;
    });

    return exited;
  }
};

#endif
