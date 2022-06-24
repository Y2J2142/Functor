#include "Functor.hpp"
#include "Identity.hpp"
#include "catch2/catch_all.hpp"
#include <fmt/core.h>
#include <list>
#include <optional>
#include <string>
#include <type_traits>
#include <vector>

using namespace Functional;
using namespace std::string_literals;
int triple(int i) { return i * 3; }

TEST_CASE("Functor construction", "[functor]") {
	std::vector ints{1, 2, 3, 4, 5};
	auto fromInts = From(ints);
}

TEST_CASE("Functor map", "[functor]") {
	auto functor = From(std::vector{1, 2, 3, 4, 5});
	auto func = [](int i) { return i * 2; };
	auto mapped = functor.Map(func).Map(triple);
	REQUIRE(mapped.UnderlyingCollection() == std::vector{6, 12, 18, 24, 30});
	auto arr = From(std::vector<int>{1, 2, 3, 4, 5});
	auto toFloats = arr.Map([](int i) { return static_cast<float>(i); });
	REQUIRE(std::is_same_v<decltype(toFloats), Functor<std::vector<float>>>);

	struct TestStruct {
		int i;
		char c;
	};
	auto ints =
		From(std::vector<TestStruct>{{1, 'a'}, {2, 'b'}, {3, 'c'}, {4, 'd'}})
			.Map(&TestStruct::i);
	REQUIRE(std::is_same_v<Functor<std::vector<int>>, decltype(ints)>);
	REQUIRE(ints == std::vector<int>{
						1,
						2,
						3,
						4,
					});
	const auto constTestStructs =
		From(std::vector<TestStruct>{{1, 'a'}, {2, 'b'}, {3, 'c'}, {4, 'd'}});
	auto chars = constTestStructs.Map(&TestStruct::c);
	REQUIRE(std::is_same_v<Functor<std::vector<char>>, decltype(chars)>);
	REQUIRE(chars.UnderlyingCollection() == std::vector{'a', 'b', 'c', 'd'});
}

TEST_CASE("Filter", "[functor]") {
	auto functor = From(std::vector{1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
	auto evens = functor.Filter([](auto i) { return i % 2 == 0; });
	REQUIRE(functor.UnderlyingCollection() ==
			std::vector{1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
	REQUIRE(evens.UnderlyingCollection() == std::vector{2, 4, 6, 8, 10});
	auto even = [](int i) { return i % 2 == 0; };
	auto inPlace =
		Functor{std::vector{1, 2, 3, 4, 5, 6, 7, 8, 9, 10}}.Filter(even);
	REQUIRE(inPlace.UnderlyingCollection() == std::vector{2, 4, 6, 8, 10});
}

TEST_CASE("Reduce", "[functor]") {
	auto sum = 1 + 2 + 3 + 4 + 5 + 6 + 7 + 8 + 9 + 10;
	auto acc = From(std::vector{1, 2, 3, 4, 5, 6, 7, 8, 9, 10})
				   .Reduce([](int a, int b) { return a + b; });
	REQUIRE(sum == acc);
}

TEST_CASE("Chunk", "[functor]") {

	auto functor = From(std::list{1, 2, 3, 4, 5, 6, 7, 8});
	auto chunked = functor.Chunk(4);
	REQUIRE(chunked.size() == 2);
	REQUIRE(chunked.UnderlyingCollection().front() == std::list{1, 2, 3, 4});
	REQUIRE(chunked.UnderlyingCollection().back() == std::list{5, 6, 7, 8});
	REQUIRE(
		std::is_same_v<decltype(chunked), Functor<std::list<std::list<int>>>>);
	auto rvalueChunked =
		From(std::vector{1, 2, 3, 4, 5, 6, 7, 8, 9, 10}).Chunk(1);
	REQUIRE(rvalueChunked.size() == 10);
	auto chunkBiggerThanOriginal = From(std::vector{1, 2, 3}).Chunk(5);
	REQUIRE(std::is_same_v<Functor<std::vector<std::vector<int>>>,
						   decltype(chunkBiggerThanOriginal)>);
}

TEST_CASE("Template Chunk", "[functor]") {
	auto functor = Functor{std::string{"aaabbbccc"}};
	auto vv = functor.Chunk<std::vector>(3);
	auto &v = vv.UnderlyingCollection();
	REQUIRE(v[0] == "aaa");
	REQUIRE(v[1] == "bbb");
	REQUIRE(v[2] == "ccc");

	auto rvalue = Functor{std::string{"KastaSquad"}}.Chunk<std::vector>(5);
	REQUIRE(rvalue[0] == "Kasta");
	REQUIRE(rvalue[1] == "Squad");
}

TEST_CASE("Any", "[functor]") {
	REQUIRE(Functor{std::vector<int>{}}.Any() == false);
	REQUIRE(Functor{std::vector{1, 2, 3}}.Any());
	REQUIRE(Functor{std::string{"xd"}}.Any('x'));
	REQUIRE(Functor{std::vector{1, 2, 3, 4}}.Any(5) == false);
	REQUIRE(
		Functor{std::vector{1, 2, 3, 4}}.Any([](int i) { return i % 2 == 0; }));
	REQUIRE(Functor{std::vector{1, 2, 3, 4}}.Any(
				[](int i) { return i == 10; }) == false);
}

TEST_CASE("Count", "[functor]") {
	auto moreThanD = [](char c) -> bool { return c > 'd'; };
	REQUIRE(From("length:8"s).Count() == 8);
	REQUIRE(From("ababbaaaaaa"s).Count('b') == 3);
	REQUIRE(From("abcdefgh"s).Count(moreThanD) == 4);
}

TEST_CASE("Sort", "[functor]") {
	auto f = From(std::vector{9, 1, 2, 8, 3, 7, 4, 6, 5});
	auto sorted = f.Sort();
	REQUIRE(sorted.UnderlyingCollection() ==
			std::vector{1, 2, 3, 4, 5, 6, 7, 8, 9});
	auto sortedInPlace = From(std::string{"cbad"}).Sort();
	REQUIRE(sortedInPlace.UnderlyingCollection() == "abcd");
	auto reverseSorted = f.Sort(std::greater<>{});
	REQUIRE(reverseSorted.UnderlyingCollection() ==
			std::vector{9, 8, 7, 6, 5, 4, 3, 2, 1});
	const auto list = Functor{std::list{5, 4, 3, 2, 1}};
	REQUIRE(list.Sort().UnderlyingCollection() == std::list{1, 2, 3, 4, 5});
	auto sortedList = From(std::list{5, 4, 3, 2, 1}).Sort();
	REQUIRE(sortedList.UnderlyingCollection() == std::list{1, 2, 3, 4, 5});
}

TEST_CASE("Unique", "[functor]") {
	auto f = From(std::vector{1, 1, 1, 1, 1, 1});
	auto unique = f.Unique();
	REQUIRE(unique.UnderlyingCollection() == std::vector{1});
	auto list = From(std::list{1, 2, 1, 3, 3, 4, 6, 3});
	auto listUnique = list.Unique();
	REQUIRE(listUnique == std::list{1, 2, 3, 4, 6});
	auto uniqueRvalue = From("aabbcc"s).Unique();
	REQUIRE(uniqueRvalue == "abc"s);
}

TEST_CASE("Find", "[functor]") {

	auto f = From(std::vector{1, 2, 3, 4, 5});
	auto five = f.Find(5);
	REQUIRE(five == 5);
	auto exceptionCaught = false;
	try {
		f.Find(6);
	} catch (const std::runtime_error &) {
		exceptionCaught = true;
	}
	REQUIRE(exceptionCaught == true);

	auto firstEven = f.Find([](int i) { return i % 2 == 0; });
	REQUIRE(firstEven == 2);

	exceptionCaught = false;
	try {
		f.Find([](int i) { return i > 10; });
	} catch (const std::runtime_error &) {
		exceptionCaught = true;
	}
	REQUIRE(exceptionCaught == true);
}

TEST_CASE("Index Operator", "[functor]") {
	auto functor = Functor{std::vector{1, 2, 3, 4, 5}};
	REQUIRE(functor[0] == 1);
	REQUIRE(functor[1] == 2);
	REQUIRE(functor[2] == 3);
	REQUIRE(functor[3] == 4);
	REQUIRE(functor[4] == 5);
}

TEST_CASE("Average", "[functor]") {
	auto functor = From(std::vector{2, 2, 2, 4, 4, 4});
	REQUIRE(functor.Average() == 3);
}

TEST_CASE("AverageBy", "[functor]") {
	auto functor = From(std::vector{2, 2, 2, 4, 4, 4});
	REQUIRE(functor.Average([](int i) { return i * 2; }, 0) == 6);
	auto strings = From(std::vector{"abc"s, "abcde"s});
	REQUIRE(strings.Average([](const std::string &s) { return s.size(); }) ==
			4);
}

TEST_CASE("Flatten", "[functor]") {
	auto functor = From(std::vector{std::vector{std::vector{1}},
									std::vector{std::vector{2}},
									std::vector{std::vector{3}}});
	auto flat = functor.Flatten();
	REQUIRE(std::is_same_v<decltype(flat), Functor<std::vector<int>>>);
	REQUIRE(flat == std::vector{1, 2, 3});
}

TEST_CASE("Collect", "[functor]") {
	auto functor = Functor{std::vector{1, 1, 1}};
	auto mapped = functor.Collect([](int) { return std::vector{1, 2, 3}; });
	REQUIRE(mapped == std::vector{1, 2, 3, 1, 2, 3, 1, 2, 3});

	auto string = From(std::vector{"abc"s, "def"s});
	auto collected = string.Collect([](const std::string &s) {
		std::vector<char> out{};
		std::copy(s.begin(), s.end(), std::back_inserter(out));
		return out;
	});
	REQUIRE(collected == std::vector{'a', 'b', 'c', 'd', 'e', 'f'});
}

TEST_CASE("ForAll", "[functor]") {
	auto f1 = Functor{std::vector{2, 4, 6, 8}};
	auto f2 = Functor{std::vector{2, 3, 6, 8}};
	auto isEven = [](int i) { return i % 2 == 0; };
	REQUIRE(f1.ForAll(isEven) == true);
	REQUIRE(f2.ForAll(isEven) == false);
}

TEST_CASE("Head", "[functor]") {
	auto functor = Functor{std::list{1, 2, 3}};
	auto empty = Functor{std::string{}};
	REQUIRE(functor.Head() == 1);
	auto exceptionThrown = false;
	try {
		empty.Head();
	} catch (EmptyFunctorException &) {
		exceptionThrown = true;
	}
	REQUIRE(exceptionThrown == true);
	auto headOfRValue = From("abs"s).Head();
	REQUIRE(headOfRValue == 'a');
	exceptionThrown = false;
	try {
		From(""s).Head();
	} catch (EmptyFunctorException &) {
		exceptionThrown = true;
	}
	REQUIRE(exceptionThrown == true);
}

TEST_CASE("Tail", "[functor]") {
	auto f = Functor{"abc"s};
	auto fTail = f.Tail();
	auto empty = Functor{std::string{}};
	REQUIRE(fTail == "bc"s);
	auto exceptionThrown = false;
	try {
		empty.Tail();
	} catch (EmptyFunctorException &) {
		exceptionThrown = true;
	}
	REQUIRE(exceptionThrown == true);
	auto tailOfRValue = From("abs"s).Tail();
	REQUIRE(std::is_same_v<decltype(tailOfRValue), Functor<std::string>>);
	REQUIRE(tailOfRValue == "bs"s);
	exceptionThrown = false;
	try {
		From(""s).Tail();
	} catch (EmptyFunctorException &) {
		exceptionThrown = true;
	}
	REQUIRE(exceptionThrown == true);
}

TEST_CASE("Add", "[functor]") {
	auto f = Functor{""s};
	f.Add('a').Add('b').Add('c').Add('d');
	REQUIRE(f == "abcd"s);
	auto add = From(""s).Add('a').Add('b').Add('c').Add('d');
	REQUIRE(add == "abcd"s);

	const auto functor = Functor{"ab"s};
	auto extended = functor.Add('c');
	REQUIRE(functor == "ab"s);
	REQUIRE(extended == "abc"s);
}

TEST_CASE("Zip", "[functor]") {
	auto f1 = Functor{std::vector{1, 2, 3}};
	auto f2 = Functor{"abc"s};
	auto f3 = Functor{std::list{true, false, true}};
	auto zippped = Zip<std::vector>(f1, f2, f3);
}

TEST_CASE("WithIndex", "[functor]") {
	auto functor = Functor{std::vector{'a', 'b', 'c'}};
	auto indexed = functor.WithIndex();
	for (auto &&[idx, elem] : indexed) {
		fmt::print("index: {} elem {}", idx, elem);
	}
	REQUIRE(
		std::is_same_v<decltype(indexed)::ValueType, std::pair<size_t, char>> ==
		true);
}

TEST_CASE("StripIndex", "[functor]") {
	auto functor = Functor{std::vector{'a', 'b', 'c'}};
	auto indexed = functor.WithIndex();
	auto strip = indexed.StripIndex();
	REQUIRE(strip == functor);
	REQUIRE(std::is_same_v<decltype(functor), decltype(strip)> == true);
}

TEST_CASE("Max", "[functor]") {
	auto f = From(std::vector{1, 10, 2, 5, 4, 100});
	REQUIRE(f.Max() == 100);
	auto x = From(std::vector{""s, ""s, "a"s}).Max();
	REQUIRE(x == "a"s);
	auto const ff = From(std::vector{1, 10, 2, 5, 4, 100});
	auto xx = ff.Max();
	REQUIRE(xx == 100);
}

TEST_CASE("MaxBy", "[functor]") {
	std::vector<std::pair<int, int>> vec{{1, 2}, {2, 4}, {10, 1}};
	auto f = From(vec);
	auto maxl = f.Max(&decltype(vec)::value_type::first);
	auto maxr = f.Max(&decltype(vec)::value_type::second);
	REQUIRE(maxl.first == 10);
	REQUIRE(maxr.second == 4);
}

TEST_CASE("Min", "[functor]") {
	auto f = From(std::vector{1, 10, 2, 5, 4, 100});
	REQUIRE(f.Min() == 1);
	auto x = From(std::vector{""s, ""s, "a"s}).Min();
	REQUIRE(x == ""s);
	auto const ff = From(std::vector{1, 10, 2, 5, 4, 100});
	auto xx = ff.Min();
	REQUIRE(xx == 1);
}

TEST_CASE("MinBy", "[functor]") {
	std::vector<std::pair<int, int>> vec{{1, 2}, {2, 4}, {10, 1}};
	auto f = From(vec);
	auto maxl = f.Min(&decltype(vec)::value_type::first);
	auto maxr = f.Min(&decltype(vec)::value_type::second);
	REQUIRE(maxl.first == 1);
	REQUIRE(maxr.second == 1);
}

TEST_CASE("Choose", "[functor]") {
	auto functor = From(std::vector{1, 2, 3, 4, 5});
	auto filter = [](int i) -> std::optional<int> {
		if (i & 1)
			return {i};
		return {};
	};
	auto filtered = functor.Choose(filter);
	REQUIRE(filtered == std::vector{1, 3, 5});
}
