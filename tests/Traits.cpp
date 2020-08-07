#include "Traits.hpp"
#include "catch2/catch.hpp"
#include <fmt/core.h>
#include <list>
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
	REQUIRE(IsStdArray<floatArr>::value);
	REQUIRE(IsStdArray<float>::value == false);
	REQUIRE(IsStdArrayV<int[4]> == false);
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
	REQUIRE(NotTypeV<std::false_type>);
	REQUIRE(NotTypeV<std::true_type> == false);
	struct Derived : std::true_type {};
	REQUIRE(NotTypeV<Derived> == false);
}

TEST_CASE("Not function", "[trait]") {
	REQUIRE(Not(false));
	REQUIRE(Not(true) == false);
}

TEST_CASE("Const Iterable", "[trait]") {
	REQUIRE(ConstIterable<std::vector<int>>);
	REQUIRE(ConstIterable<int> == false);
}

TEST_CASE("Inner value", "[trait]") {
	REQUIRE(InnerValue<std::vector<int>>);
	REQUIRE(InnerValue<int> == false);
}

TEST_CASE("Erasable", "[trait]") {
	REQUIRE(Erasable<int> == false);
	REQUIRE(Erasable<std::vector<int>>);
}

TEST_CASE("Equality comparable", "[trait]") {
	REQUIRE(EqualityComparable<int>);
	struct NonComparable {};
	REQUIRE(EqualityComparable<NonComparable> == false);
}

TEST_CASE("Binary predicate", "[trait]") {
	REQUIRE(BinaryPredicate<std::less<int>, int, int>);
	auto lambda = [](int a, int b) { return std::string{}; };
	REQUIRE(BinaryPredicate<decltype(lambda), int, int> == false);
}
TEST_CASE("StdSortable", "[trait]") {
	REQUIRE(StdSortable<std::vector<int>>);
	REQUIRE(StdSortable<std::list<int>> == false);
	// REQUIRE(StdSortable<float> == false);
	// REQUIRE(StdSortable<std::string>);
}

TEST_CASE("MethodSortable", "[trait]") {
	REQUIRE(MethodSortable<std::vector<int>> == false);
	REQUIRE(MethodSortable<std::list<int>>);
	struct Test {
		void sort();
	};
	REQUIRE(MethodSortable<Test>);
}

TEST_CASE("Sortable", "[trait]") {
	REQUIRE(Sortable<std::vector<int>>);
	REQUIRE(Sortable<std::string>);
	REQUIRE(Sortable<std::list<int>>);
	REQUIRE(Sortable<int> == false);
}