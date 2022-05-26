#pragma once
#include "Traits.hpp"
#include <type_traits>

namespace Functional {

template <class F, class G>
auto operator>>(F f, G g) {

	return [=]<class... Args>
	requires Chainable<F, G, Args...>(Args && ...args) {
		return std::invoke(g, std::invoke(f, std::forward<Args>(args)...));
	};
}

} // namespace Functional
