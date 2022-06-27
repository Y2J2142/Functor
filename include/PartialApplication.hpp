#pragma once
#include <functional>
#include <utility>
namespace Functional {

template <class F, class Arg>
constexpr auto operator>=(F &&f, Arg &&arg) {
	return std::bind_front(std::forward<F>(f), std::forward<Arg>(arg));
}

} // namespace Functional
