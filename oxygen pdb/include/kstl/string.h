#pragma once
#include <string>
#include "allocator.h"

namespace kstd
{
    template <typename T, typename TAllocator = Allocator<T>>
    using basic_string = std::basic_string<T, std::char_traits<T>, TAllocator>;

    using string = basic_string<char, Allocator<char>>;

    using wstring = basic_string<wchar_t, Allocator<wchar_t>>;
    template<typename T,POOL_TYPE type,ULONG tag>
    using _string = basic_string<char, _Allocator<char, type, tag>>;
}