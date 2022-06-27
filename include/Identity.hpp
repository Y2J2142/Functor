#pragma once
#include <utility>
namespace Functional {

template <typename T>
T &&Identity(T &&t) {
	return std::forward<T>(t);
}

} // namespace Functional