#pragma once
#include <memory>
#include "jxy_allocator.h"


//这个主要是定义Allocator和DefaultDelete
namespace kstd
{
	template<typename T,POOL_TYPE PTYPE,ULONG tag>
	using _Allocator = jxy::details::allocator<T, PTYPE, tag>;
	template<typename T>
	using Allocator =jxy::allocator<T>;
	template<typename T>
	using DefaultDelete = jxy::default_delete<T>;

}