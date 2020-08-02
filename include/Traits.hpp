#pragma once
#include <type_traits>
namespace Functional {

template <typename Test, template <typename...> typename Ref>
struct IsSpecialization : std::false_type {};

template <template <typename...> class Ref, typename... Args>
struct IsSpecialization<Ref<Args...>, Ref> : std::true_type {};

template <typename...>
struct SwapTemplateParameter {};

template <typename NewType, template <typename...> typename Base,
		  typename OldType, typename... Args>
struct SwapTemplateParameter<NewType, Base<OldType, Args...>> {
	using Type = Base<NewType, Args...>;
};
template <typename NewType, template <typename...> typename Base,
		  typename OldType>
struct SwapTemplateParameter<NewType, Base<OldType>> {
	using Type = Base<NewType>;
};

} // namespace Functional