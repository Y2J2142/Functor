#pragma once
#include <type_traits>
namespace Functional {

template <typename Test, template <typename...> typename Ref>
struct IsSpecialization : std::false_type {};

template <template <typename...> class Ref, typename... Args>
struct IsSpecialization<Ref<Args...>, Ref> : std::true_type {};

} // namespace Functor