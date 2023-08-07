 //
// Copyright (c) Johnny Shaw. All rights reserved.
// 
// File:     jxystl/alloc.cpp
// Author:   Johnny Shaw
// Abstract: cpp new/delete functions
//
// Default new/delete is intentionally unimplemented! This forces specifying
// the pool type and tags for all allocations.
//
#include "alloc.h"

#include <stdexcept>

void* __cdecl operator new(size_t Size, POOL_TYPE PoolType, ULONG PoolTag) noexcept
{
    if (Size == 0)
    {
        Size = 1;
    }

#pragma warning(push)
#pragma warning(disable : 4996) // FIxME - deprecated function
    return ExAllocatePoolWithTag(PoolType, Size, PoolTag);
#pragma warning(pop)
}

void __cdecl operator delete(void* Memory, POOL_TYPE, ULONG PoolTag) noexcept
{
    if (Memory)
    {
        ExFreePoolWithTag(Memory, PoolTag);
    }
}

void* __cdecl operator new[](size_t Size, POOL_TYPE PoolType, ULONG PoolTag) noexcept
{
    return operator new(Size, PoolType, PoolTag);
}

void __cdecl operator delete[](void* Memory, POOL_TYPE PoolType, ULONG PoolTag) noexcept
{
    return operator delete(Memory, PoolType, PoolTag);
}

// for std::destroy_at
void __cdecl operator delete(void*, size_t)
{
    KeBugCheck(0x1008);
}