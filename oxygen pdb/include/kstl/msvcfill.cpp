//
// Copyright (c) Johnny Shaw. All rights reserved.
// 
// File:     jxystl/memory.hpp
// Author:   Johnny Shaw
// Abstract: MSVC fill functionality 
//
// Some MSVC functionality must be implemented for the STL containers to link
// to. I have consciously decided to implement each with care given to the
// operating environment (kernel). This means that code using the STL directly
// and not through the jxylib wrappers might encounter linker errors.
// This is done intentionally as to not introduce a path which may be unsafe
// for the kernel.
//
#include <intrin.h>
#include <fltKernel.h>


namespace std
{

void __cdecl _Xbad_function_call()
{
    KeBugCheck(0x1000);
}

void __cdecl _Xbad_alloc()
{
    KeBugCheck(0x1001);
}

void __cdecl _Xinvalid_argument(const char*)
{
    KeBugCheck(0x1002);
}

void __cdecl _Xlength_error(const char*)
{
    KeBugCheck(0x1003);
}

void __cdecl _Xout_of_range(const char*)
{
    KeBugCheck(0x1004);
}

void __cdecl _Xoverflow_error(const char*)
{
    KeBugCheck(0x1005);
}

void __cdecl _Xruntime_error(const char*)
{
    KeBugCheck(0x1006);
}

void __cdecl _Throw_Cpp_error(int)
{
    //
    // FIXME: This should throw a std::system_error but it comes with global
    // allocator requirements. For now we'll throw a faked "system error" as
    // a runtime error. 
    //
    KeBugCheck(0x1007);
}

}

// invalid_parameter.cpp
extern "C" 
__declspec(noreturn)
void __cdecl _invalid_parameter_noinfo_noreturn(void)
{
    //
    // AFAIK attempting to continue here is going to blow up regardless.
    // So, we have no choice but to bug check for now.
    //
    KeBugCheckEx(
        static_cast<ULONG>(STATUS_INVALID_PARAMETER),
        reinterpret_cast<ULONG_PTR>(_ReturnAddress()),
        reinterpret_cast<ULONG_PTR>(_AddressOfReturnAddress()),
        0, 
        0);
}

extern "C"
__declspec(noreturn) 
void __cdecl abort()
{
    KeBugCheckEx(
        static_cast<ULONG>(STATUS_DRIVER_PROCESS_TERMINATED),
        reinterpret_cast<ULONG_PTR>(_ReturnAddress()),
        reinterpret_cast<ULONG_PTR>(_AddressOfReturnAddress()),
        0,
        0);
}
