#pragma once
#include <compare>
#include <concepts>
#include <iterator>

template <std::integral T>
class Range {
	struct Iterator {
		T curretnValue;
		auto operator<=>(const Iterator &) const = default;
		Iterator &operator++() {
			curretnValue += 1;
			return *this;
		}
		Iterator &operator++(int) {
			auto copy = *this;
			this->curretnValue++;
			return copy;
		}
		T operator*() { return curretnValue; }

		using value_type = T;
		using reference = T &;
		using difference_type = std::size_t;
		using pointer = Iterator *;
		using iterator_category = std::forward_iterator_tag;
	};

  public:
	T Begin;
	T End;
	using value_type = T;
	using ValuerType = T;
	using reference = T &;

	Iterator begin() { return Iterator{Begin}; }

	Iterator end() { return Iterator{End}; }
	std::size_t size() const { return static_cast<std::size_t>(End - Begin); }
};
