#ifndef C3_SELECT_FWD_HPP
#define C3_SELECT_FWD_HPP

#include <type_traits>

template <typename T>
using view_type =
    conditional_t<is_rvalue_reference_v<T>, remove_reference_t<T>, T &>;

template <typename T> using key_type = typename decay_t<T>::key_type;

struct c3;
struct Select;
struct Data;
struct Transition;

#endif
