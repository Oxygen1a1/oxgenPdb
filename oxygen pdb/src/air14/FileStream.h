#pragma once
#include "ScopedHandle.h"
#include <string_view>

namespace symbolic_access
{
	class FileStream
	{
	public:
		FileStream(std::wstring_view FilePath);
		FileStream(const FileStream&) = delete;
		FileStream& operator=(const FileStream&) = delete;
		FileStream(FileStream&&) = default;
		FileStream& operator=(FileStream&&) = default;

		void Seekg(uint32_t Offset);

		bool Read(void* Buffer, uint32_t Size);

		uint32_t Tellg() const;

		operator bool()
		{
			return m_FileHandle;
		}

	private:
		ScopedHandle m_FileHandle;
		uint32_t m_Offset;
	};
}