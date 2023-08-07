#include <fltKernel.h>
#include "FileStream.h"

namespace symbolic_access
{
	FileStream::FileStream(std::wstring_view FilePath) : m_Offset(0)
	{
#ifdef _KERNEL_MODE
		UNICODE_STRING filePath{};
		RtlInitUnicodeString(&filePath, FilePath.data());

		OBJECT_ATTRIBUTES fileObjectAttributes{ sizeof(fileObjectAttributes), 0, &filePath, OBJ_KERNEL_HANDLE };
		IO_STATUS_BLOCK statusBlock{};
		HANDLE fileHandle{};
		if (!NT_SUCCESS(ZwOpenFile(&fileHandle, FILE_GENERIC_READ, &fileObjectAttributes, &statusBlock, FILE_SHARE_VALID_FLAGS, FILE_SYNCHRONOUS_IO_NONALERT)))
			return;

		m_FileHandle = fileHandle;
#else
		m_FileHandle = CreateFileW(FilePath.data(), GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_READONLY, 0);
#endif
	}

	void FileStream::Seekg(uint32_t Offset)
	{
		m_Offset = Offset;
	}

	bool FileStream::Read(void* Buffer, uint32_t Size)
	{
#ifdef _KERNEL_MODE
		IO_STATUS_BLOCK statusBlock{};
		LARGE_INTEGER offset{ m_Offset };
		if (!NT_SUCCESS(ZwReadFile(m_FileHandle.Get(), 0, 0, 0, &statusBlock, Buffer, Size, &offset, 0)))
			return false;
#else
		DWORD bytesRead{};
		LARGE_INTEGER fileOffset{};
		fileOffset.QuadPart = m_Offset;
		SetFilePointerEx(m_FileHandle.Get(), fileOffset, 0, FILE_BEGIN);
		if (!ReadFile(m_FileHandle.Get(), Buffer, Size, &bytesRead, nullptr))
			return false;
#endif

		m_Offset += Size;
		return true;
	}

	uint32_t FileStream::Tellg() const
	{
		return m_Offset;
	}

}