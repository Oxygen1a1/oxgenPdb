#pragma once
#include <vector>
#include "allocator.h"

namespace kstd
{
    template <typename T, typename TAllocator = Allocator<T>>
    using vector = std::vector<T, TAllocator>;
}