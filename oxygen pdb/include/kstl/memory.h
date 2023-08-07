#pragma once
#include "allocator.h"

namespace kstd
{
    template <typename T>
    using unique_ptr = std::unique_ptr<T, DefaultDelete<T>>;

    template <typename T, typename... TArgs, std::enable_if_t<!std::is_array_v<T>, int> = 0>
    unique_ptr<T> make_unique(TArgs&&... _Args)
    {
#ifdef _KERNEL_MODE
        return unique_ptr<T>(new(POOL_TYPE::NonPagedPool, 'syma')T(std::forward<TArgs>(_Args)...));
#else
        return unique_ptr<T>(new T(std::forward<TArgs>(_Args)...));
#endif
    }

    template <class T, std::enable_if_t<std::is_array_v<T>&& std::extent_v<T> == 0, int> = 0>
    unique_ptr<T> make_unique(const size_t Size)
    {
        using Elem = std::remove_extent_t<T>;
#ifdef _KERNEL_MODE
        return unique_ptr<T>(new(POOL_TYPE::NonPagedPool, 'syma')Elem[Size]());
#else
        return unique_ptr<T>(new Elem[Size]());
#endif
    }

    template <typename T>
    using shared_ptr = std::shared_ptr<T>;

    template <typename T, typename... TArgs>
    shared_ptr<T> make_shared(TArgs&&... Args) noexcept
    {
        //
        // Rather than use make_shared here, we'll force allocate_shared.
        // This is because there is no way to specify an allocator on make_shared 
        // thus internally it will use the global new allocator, since jxystl aims
        // to force tagging and typing of all allocations we avoid that at all
        // costs.
        // 
        // allocate_shared enables us to specify jxy::allocator with our pool
        // parameters and eliminates this problem.
        //

        using allocator = Allocator<T>;
        return std::allocate_shared<T, allocator>(allocator(), std::forward<TArgs>(Args)...);
    }
}


