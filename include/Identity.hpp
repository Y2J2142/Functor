#pragma once
#include <utility>
namespace Functional {

template <typename T>
auto Identity(T &&t) {
	return std::forward<T>(t);
}

} // namespace Functional