#include "Compose.hpp"
#include "catch2/catch_all.hpp"
#include <fmt/core.h>
#include <list>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>
using namespace Functional;

auto threeTimes(int i) { return i * 3; };
auto toPair(int i) { return std::pair{i, i}; };

TEST_CASE("Compose", "[compose]") {

	auto length = [](std::string_view str) { return str.size(); };

	auto twice = [](int i) { return 2 * i; };
	auto fg = length >> twice;
	auto test = [](std::string) {};
	auto doesNotCompile = length >> test;
	// doesNotCompile("ab");

	REQUIRE(fg("ab") == 4);
	auto composed = twice >> threeTimes >> toPair;
	auto pair = composed(1);
	auto sixes = std::pair{6, 6};
	REQUIRE(pair == sixes);
}