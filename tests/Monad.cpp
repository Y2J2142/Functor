// #include "Monad.hpp"
// #include "catch2/catch_all.hpp"
// #include <fmt/core.h>
// #include <type_traits>
// using namespace Functional;

// TEST_CASE("Monad construction", "[monad]") {
// 	Monad<int> foo;
// 	Monad bar{10};
// 	auto baz = Monad{Monad{Monad{10}}};
// 	REQUIRE(std::is_same_v<decltype(baz), Monad<int>>);
// }
// TEST_CASE("Monad apply", "[monad]") {
// 	auto foo = Monad{10};
// 	auto dbl = [](int m) { return m * 2; };
// 	REQUIRE(foo.Apply(dbl).Value() == foo.Value() * 2);
// 	auto bar = Monad<int>{};
// 	REQUIRE(bar.Apply(dbl).HasValue() == false);
// }