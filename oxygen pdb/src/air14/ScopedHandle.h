#pragma once

namespace symbolic_access
{
	class ScopedHandle
	{
	public:
		ScopedHandle(void* Handle = 0);
		ScopedHandle(ScopedHandle&& Other);
		ScopedHandle& operator=(ScopedHandle&& Other);
		~ScopedHandle();

		ScopedHandle& operator=(const ScopedHandle&) = delete;
		ScopedHandle(const ScopedHandle&) = delete;

		void Reset(void* Handle = 0);

		void* Release();

		void* Get();

		void WaitFor() const;

		operator bool() const;
	private:
		void* m_Handle;
	};
}