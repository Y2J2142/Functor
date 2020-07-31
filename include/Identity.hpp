#pragma once
#include <utility>
namespace Functor {

template <typename T>
decltype(auto) Identity(T &&t) {
  return std::forward<T>(t);
}

} // namespace Functor