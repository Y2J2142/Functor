#pragma once
#include "Traits.hpp"
#include <functional>

namespace Functional {

template <class F, class Arg>
auto operator>(Arg &&arg, F &&f) {
	return std::bind_front(std::forward<F>(f), std::forward<Arg>(arg));
}

template <class Arg, class... Args, class Ret,
		  class Func = std::function<Ret(Arg, Args...)>>
auto operator>(Arg &&arg, Func &&func) {
	return std::bind_front(std::forward<Func>(func), std::forward<Arg>(arg));
}

} // namespace Functional