#include "Monad.hpp"
#include "catch2/catch.hpp"
#include <fmt/core.h>
#include <type_traits>
using namespace Functor;
TEST_CASE("Monad testing", "[monad]") {
	Monad<int> foo;
	Monad bar{10};
	auto baz = Monad{Monad{Monad{10}}};
	REQUIRE(std::is_same_v<decltype(baz), Monad<int>>);
	auto dbl = [](int m) { return m * 2; };
	auto applied = ApplyFunc(bar, dbl);
	fmt::print("Is foo full : {}", foo.HasValue());
	auto appliedFoo = ApplyFunc(foo, dbl);
	REQUIRE(applied.Value() == (bar.Value() * 2));
}