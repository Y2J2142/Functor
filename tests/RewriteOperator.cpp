#include "Identity.hpp"
#include "RewriteOperator.hpp"
#include "Traits.hpp"
#include "catch2/catch_all.hpp"
#include <fmt/core.h>
#include <list>
#include <string>
#include <type_traits>
#include <vector>

using namespace Functional;
auto sub_fn(int a, int b) { return a - b; }

TEST_CASE("Rewrite Operator with lambda") {
	auto f = [](int a, int b) { return a * b; };
	auto times10 = 10 > f;
	REQUIRE(times10(5) == f(10, 5));
}

TEST_CASE("Rewrite Operator with function") {
	auto f = 10 > _w(sub_fn);
	REQUIRE(f(5) == sub_fn(10, 5));
}

TEST_CASE("Rewrite Operator chain") {
	auto f = [](int a, int b, int c) { return a + b * c; };
	auto call = 2 > (5 > (10 > f));
	REQUIRE(call() == f(10, 5, 2));
}