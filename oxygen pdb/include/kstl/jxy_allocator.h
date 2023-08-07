#pragma once
#include "alloc.h"
#include <fltKernel.h>
#include <memory>

namespace jxy
{

namespace details
{

template <typename T, POOL_TYPE t_PoolType, ULONG t_PoolTag>
class allocator
{
public:

    static_assert(!std::is_const_v<T>, 
                  "The C++ Standard forbids containers of const elements "
                  "because allocator<const T> is ill-formed.");

    static constexpr POOL_TYPE pool_type = t_PoolType;
    static constexpr ULONG pool_tag = t_PoolTag;

    using _From_primary = allocator;

    using value_type = T;

    using size_type = size_t;
    using difference_type = ptrdiff_t;

    using propagate_on_container_move_assignment = std::true_type;

    constexpr allocator() noexcept {}

    constexpr allocator(const allocator&) noexcept = default;

    template <typename Other>
    constexpr allocator(const allocator<Other, t_PoolType, t_PoolTag>&) noexcept
    {
    }

    _CONSTEXPR20 ~allocator() = default;
    _CONSTEXPR20 allocator& operator=(const allocator&) = default;

    _CONSTEXPR20
    void deallocate(
        value_type* const Memory,
        const size_type)
    {
        if (Memory)
        {
            ExFreePoolWithTag(Memory, t_PoolTag);
        }
    }

    _NODISCARD _CONSTEXPR20
    __declspec(allocator)
    value_type* allocate(_CRT_GUARDOVERFLOW const size_type Count)
    {
#pragma warning(push)
#pragma warning(disable : 4996) // FIXME - deprecated function
        return static_cast<value_type*>(
            ExAllocatePoolWithTag(t_PoolType,
                                  (sizeof(value_type) * Count),
                                  t_PoolTag));
#pragma warning(pop)
    }


    template <typename Other>
    struct rebind
    {
        using other = allocator<Other, t_PoolType, t_PoolTag>;
    };

#if _HAS_DEPRECATED_ALLOCATOR_MEMBERS
    _CXX17_DEPRECATE_OLD_ALLOCATOR_MEMBERS typedef T* pointer;
    _CXX17_DEPRECATE_OLD_ALLOCATOR_MEMBERS typedef const T* const_pointer;

    _CXX17_DEPRECATE_OLD_ALLOCATOR_MEMBERS typedef T& reference;
    _CXX17_DEPRECATE_OLD_ALLOCATOR_MEMBERS typedef const T& const_reference;

    using is_always_equal _CXX17_DEPRECATE_OLD_ALLOCATOR_MEMBERS = std::true_type;

    _CXX17_DEPRECATE_OLD_ALLOCATOR_MEMBERS 
    _NODISCARD 
    T* address(T& Val) const noexcept 
    {
        return std::addressof(Val);
    }

    _CXX17_DEPRECATE_OLD_ALLOCATOR_MEMBERS 
    _NODISCARD
    const T* address(const T& Val) const noexcept 
    {
        return std::addressof(Val);
    }

    _CXX17_DEPRECATE_OLD_ALLOCATOR_MEMBERS _NODISCARD 
    __declspec(allocator) 
    T* allocate(_CRT_GUARDOVERFLOW const size_type _Count, const void*) 
    {
        return allocate(_Count);
    }

    template <class U, class... TArgs>
    _CXX17_DEPRECATE_OLD_ALLOCATOR_MEMBERS 
    void construct(U* const Memory, TArgs&&... Args) 
    {
        ::new (std::_Voidify_iter(Memory)) U(std::forward<TArgs>(Args)...);
    }

    template <class U>
    _CXX17_DEPRECATE_OLD_ALLOCATOR_MEMBERS 
    void destroy(U* const Memory) 
    {
        Memory->~U();
    }

    _CXX17_DEPRECATE_OLD_ALLOCATOR_MEMBERS 
    _NODISCARD 
    size_type max_size() const noexcept 
    {
        return static_cast<size_type>(-1) / sizeof(T);
    }
#endif // _HAS_DEPRECATED_ALLOCATOR_MEMBERS

};

template <typename T, POOL_TYPE t_PoolType, ULONG t_PoolTag>
class default_delete
{
public:

    static constexpr POOL_TYPE pool_type = t_PoolType;
    static constexpr ULONG pool_tag = t_PoolTag;

    using value_type = T;

    constexpr default_delete() noexcept = default;

    template <typename Other, std::enable_if_t<
                              (std::is_convertible_v<T*, typename Other::value_type*> &&
                              (t_PoolType == Other::pool_type) &&
                              (t_PoolTag == Other::pool_tag)), int> = 0>
    default_delete(const default_delete<typename Other::value_type, 
                                        Other::pool_type, 
                                        Other::pool_tag>&) noexcept 
    {
    }

    void operator()(T* Pointer) const noexcept
    {
        static_assert(0 < sizeof(Pointer), "can't delete an incomplete type");
        if (Pointer)
        {
            Pointer->~T();
            ExFreePoolWithTag(Pointer, t_PoolTag);
        }
    }

};

template <typename T, POOL_TYPE t_PoolType, ULONG t_PoolTag>
struct default_delete<T[], t_PoolType, t_PoolTag> // default deleter for unique_ptr to array of unknown size
{
    static constexpr POOL_TYPE pool_type = t_PoolType;
    static constexpr ULONG pool_tag = t_PoolTag;

    using value_type = T;

    constexpr default_delete() noexcept = default;

    template <typename Other, std::enable_if_t<
        (std::is_convertible_v<T(*)[], typename Other::value_type(*)[]> &&
            (t_PoolType == Other::pool_type) &&
            (t_PoolTag == Other::pool_tag)), int> = 0>
        default_delete(const default_delete<typename Other::value_type,
            Other::pool_type,
            Other::pool_tag>&) noexcept
    {
    }


    template <typename Ut, std::enable_if_t<std::is_convertible_v<Ut(*)[], T(*)[]>, int> = 0>
    void operator()(Ut* Pointer) const noexcept
    {
        static_assert(0 < sizeof(Ut), "can't delete an incomplete type");
        if (Pointer)
        {
            //const auto n = *(reinterpret_cast<size_t*>(Pointer) - 1);
            //for (size_t i{}; i < n; ++i)
            //    Pointer[i].~T();
            ExFreePoolWithTag(Pointer, t_PoolTag);
        }
    }
};

}

template <typename T>
using allocator = details::allocator<T, POOL_TYPE::NonPagedPool, 'oxyg'>;

template <typename T>
using default_delete = details::default_delete<T, POOL_TYPE::NonPagedPool, 'oxyg'>;

}
