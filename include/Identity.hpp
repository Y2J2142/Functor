#pragma once
#include <utility>
namespace Functional {

template <typename T>
decltype(auto) Identity(T &&t) {
	return std::forward<T>(t);
}

} // namespace Functional