#pragma once

#include <cstddef>
#include <proxy/proxy.h>
#include <string_view>
#include <tuple>

namespace grafixboard::type_traits
{

template <typename T>
struct GetInnerTypes;

template <template <typename...> class TemplateClass, typename... InnerTypes>
struct GetInnerTypes<TemplateClass<InnerTypes...>>
{
    using types = std::tuple<InnerTypes...>;
};

template <typename T>
using GetInnerTypes_t = typename GetInnerTypes<T>::types;

template <typename T, std::size_t N = 0>
using GetInnerType_t = std::tuple_element_t<N, GetInnerTypes_t<T>>;

/**
 * @brief Return the type name of the template parameter.
 */
template <typename T>
constexpr auto typeName() -> std::string_view
{
#if defined(__clang__)
    std::string_view name = __PRETTY_FUNCTION__;
    constexpr std::string_view kPrefix =
        "std::string_view yutils::type_traits::typeName() [T = ";
    constexpr std::string_view kSuffix = "]";
#elif defined(__GNUC__)
    std::string_view name = __PRETTY_FUNCTION__;
    constexpr std::string_view kPrefix =
        "constexpr std::string_view yutils::type_traits::typeName() [with T = ";
    constexpr std::string_view kSuffix =
        "; std::string_view = std::basic_string_view<char>]";
#elif defined(_MSC_VER)
    std::string_view name = __FUNCSIG__;
    constexpr std::string_view kPrefix =
        "class std::basic_string_view<char,struct std::char_traits<char> "
        "> __cdecl yutils::type_traits::typeName<";
    constexpr std::string_view kSuffix = ">(void)";
#else
    #error "Unsupported compiler."
#endif
    name.remove_prefix(kPrefix.size());
    name.remove_suffix(kSuffix.size());
    return name;
}

}  // namespace grafixboard::type_traits