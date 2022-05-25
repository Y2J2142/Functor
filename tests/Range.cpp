#include "Functor.hpp"
#include "Identity.hpp"
#include "Range.hpp"
#include "catch2/catch_all.hpp"
#include <fmt/core.h>
#include <list>
#include <string>
#include <type_traits>
#include <vector>

using namespace std::string_literals;

TEST_CASE("Range", "[Range]") {
	std::vector<std::size_t> ints{};
	ints.reserve(10);
	Range rng{1, 10};
	std::copy(rng.begin(), rng.end(), std::back_inserter(ints));
	REQUIRE(ints == std::vector<std::size_t>{1, 2, 3, 4, 5, 6, 7, 8, 9});
	REQUIRE(ints.size() == 9);
}

TEST_CASE("Range zip", "[Range]") {
	auto func = Functional::From(std::vector{"ab"s, "cd"s, "ef"s, "gh"s});
	auto rng = Functional::From(Range{1, 5});
	auto withIndex = Functional::Zip<std::vector>(func, rng);
}
