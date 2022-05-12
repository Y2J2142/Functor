#include "Identity.hpp"
#include "catch2/catch_all.hpp"
#include <type_traits>
using namespace Functional;
TEST_CASE("Identity function testing", "[id]") {
	REQUIRE(Identity(int{}) == int{});
	REQUIRE(Identity(10.f) == 10.f);
	REQUIRE(std::same_as<decltype(Identity(2.0)), decltype(2.0)>);
}