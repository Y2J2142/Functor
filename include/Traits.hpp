#pragma once
#include <type_traits>
namespace Functor {

template <typename Test, template <typename...> typename Ref>
struct is_specialization : std::false_type {};

template <template <typename...> class Ref, typename... Args>
struct is_specialization<Ref<Args...>, Ref> : std::true_type {};

} // namespace Functor