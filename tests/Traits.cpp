#include "Traits.hpp"
#include "catch2/catch.hpp"
#include <fmt/core.h>
#include <vector>
using namespace Functional;
TEST_CASE("IsSpecialization testing", "[trait]") {
	std::vector v{1, 2, 3};
	REQUIRE(IsSpecialization<decltype(v), std::vector>::value);
	REQUIRE(IsSpecialization<int, std::vector>::value == false);
}
TEST_CASE("SwapTemplate testing", "[trait]") {
	using floatVector = SwapTemplateParameter<float, std::vector<int>>::Type;
	REQUIRE(std::is_same_v<floatVector, std::vector<float>>);
}

TEST_CASE("IsStdArray testing", "[trait]") {
	using floatArr = std::array<float, 1>;
	auto isArr = IsStdArray<floatArr>::value;
	REQUIRE(IsStdArray<floatArr>::value);
	REQUIRE(IsStdArray<float>::value == false);
	REQUIRE(IsStdArray_v<int[4]> == false);
}

TEST_CASE("Bool concept", "[trait]") {
	REQUIRE(Bool<int>);
	REQUIRE(Bool<std::vector<int>> == false);
}

TEST_CASE("BooleanType concept", "[trait]") {
	REQUIRE(BooleanType<std::true_type>);
	REQUIRE(BooleanType<std::false_type>);
	struct Derived : std::true_type {};
	REQUIRE(BooleanType<Derived>);
	REQUIRE(BooleanType<char> == false);
	REQUIRE(BooleanType<char> == false);
}

TEST_CASE("NotType struct", "[trait]") {
	REQUIRE(NotType_v<std::false_type>);
	REQUIRE(NotType_v<std::true_type> == false);
	struct Derived : std::true_type {};
	REQUIRE(NotType_v<Derived> == false);
}

TEST_CASE("Not function", "[trait]") {
	REQUIRE(Not(false));
	REQUIRE(Not(true) == false);
}