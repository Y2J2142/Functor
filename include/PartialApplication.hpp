#pragma once
#include "Traits.hpp"
#include <functional>
#include <utility>
namespace Functional {

template <class F, class Arg>
constexpr auto operator>=(F &&f, Arg &&arg) {
	return std::bind_front(std::forward<F>(f), std::forward<Arg>(arg));
}

template <class F, class Arg>
constexpr auto operator>=(wrap<F> &&f, Arg &&arg) {
	return std::bind_front(std::forward<wrap<F>>(f), std::forward<Arg>(arg));
}

} // namespace Functional
