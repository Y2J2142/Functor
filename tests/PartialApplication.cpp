#include "Identity.hpp"
#include "PartialApplication.hpp"
#include "Traits.hpp"
#include "catch2/catch_all.hpp"
#include <fmt/core.h>
#include <list>
#include <string>
#include <type_traits>
#include <vector>

using namespace Functional;
auto sub(int a, int b) { return a - b; }

TEST_CASE("Partial Application with lambda", "[partial]") {
	auto add = [](int x, int y) { return x + y; };
	constexpr auto add10 = add >= 10;

	REQUIRE(add10(5) == add(10, 5));
}

TEST_CASE("Partial Application with functions", "[partial]") {
	auto subtractFrom10 = _w(sub) >= 10;
	REQUIRE(subtractFrom10(5) == sub(10, 5));
}

TEST_CASE("Partial Application with std::function", "[partial]") {
	auto sub10 = std::function{sub} >= 10;
	REQUIRE(sub10(5) == sub(10, 5));
}