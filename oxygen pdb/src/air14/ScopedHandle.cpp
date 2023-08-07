#include <fltKernel.h>
#include "ScopedHandle.h"

namespace symbolic_access
{
	ScopedHandle::ScopedHandle(void* Handle) : m_Handle(Handle)
	{
	}

	ScopedHandle::ScopedHandle(ScopedHandle&& Other)
	{
		m_Handle = Other.Release();
	}

	ScopedHandle& ScopedHandle::operator=(ScopedHandle&& Other)
	{
		Reset(Other.Release());
		return *this;
	}

	ScopedHandle::~ScopedHandle()
	{
		if (m_Handle)
#ifdef _KERNEL_MODE
			ObCloseHandle(m_Handle, KernelMode);
#else
			CloseHandle(m_Handle);
#endif
	}

	void ScopedHandle::Reset(void* Handle)
	{
#ifdef _KERNEL_MODE
		ObCloseHandle(m_Handle, KernelMode);
#else
		CloseHandle(m_Handle);
#endif
		m_Handle = Handle;
	}

	HANDLE ScopedHandle::Release()
	{
		auto handle = m_Handle;
		m_Handle = nullptr;
		return handle;
	}

	HANDLE ScopedHandle::Get()
	{
		return m_Handle;
	}

	void ScopedHandle::WaitFor() const
	{
#ifdef _KERNEL_MODE
		ZwWaitForSingleObject(m_Handle, false, nullptr);
#else
		WaitForSingleObject(m_Handle, INFINITE);
#endif
	}

	ScopedHandle::operator bool() const
	{
		return m_Handle && m_Handle != reinterpret_cast<void*>(-1);
	}
}