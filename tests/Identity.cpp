#include "Identity.hpp"
#include "catch2/catch.hpp"
using namespace Functor;
TEST_CASE("Identity function testing", "[id]") {
	REQUIRE(Identity(int{}) == int{});
	REQUIRE(Identity(float{10.f}) == float{10.f});
}