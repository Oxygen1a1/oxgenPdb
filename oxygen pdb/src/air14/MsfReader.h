#pragma once
#include "FileStream.h"
#include <kstl/vector.h>
#include <array>

namespace symbolic_access
{
	class MsfReader
	{
	public:
		enum KnownStreams
		{
			Root,
			Streams,
			TypeInfoStream,
			DebugInfo
		};

		MsfReader(FileStream FileStream);
		MsfReader(const MsfReader&) = delete;
		MsfReader& operator=(const MsfReader&) = delete;
		MsfReader(MsfReader&&) = default;
		MsfReader& operator=(MsfReader&&) = default;

		bool Initialize();

		kstd::vector<char> GetStream(size_t Index);

	private:
		uint32_t GetNumberOfPages(uint32_t Size);

		bool HasNullOffsets(const kstd::vector<uint32_t>& Offsets);

		struct ContentStream
		{
			uint32_t Size;
			kstd::vector<uint32_t> PageIndices;
		};

		struct MsfHeader
		{
			std::array<char, 32> Signature;
			uint32_t PageSize;
			uint32_t FreePageMap;
			uint32_t PageCount;
			uint32_t DirectorySize;
			uint32_t Reserved;
		};

		FileStream m_FileStream;
		uint32_t m_PageSize;
		kstd::vector<ContentStream> m_Streams;
		static constexpr std::array<char, 32> m_Signature{ "Microsoft C/C++ MSF 7.00\r\n\032DS\0\0" };
	};
}