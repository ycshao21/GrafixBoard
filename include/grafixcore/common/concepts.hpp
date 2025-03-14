#pragma once
#include <proxy/proxy.h>

namespace grafix
{

template <typename ProxyType, typename Facade>
concept proxiable = pro::proxiable<
    pro::details::allocated_ptr<ProxyType, std::allocator<ProxyType>>, Facade>;
}