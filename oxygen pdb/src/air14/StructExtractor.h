#pragma once
#include "MsfReader.h"
#include <kstl/map.h>
#include "PdbStructs.h"
#include <kstl/vector.h>
#include <kstl/string.h>
#include "ModuleData.h"
#include "StreamReader.h"

namespace symbolic_access
{

	class StructExtractor
	{
	public:
		StructExtractor(MsfReader& MsfReader);
		StructExtractor(const StructExtractor&) = delete;
		StructExtractor& operator=(const StructExtractor&) = delete;
		StructExtractor(StructExtractor&&) = default;
		StructExtractor& operator=(StructExtractor&&) = default;

		StructsMap Extract();

	private:
		class StreamReaderOffsetBackup
		{
		public:
			StreamReaderOffsetBackup(StreamReader& Reader) : m_Reader(Reader)
			{
				m_Offset = m_Reader.Tellg();
			}
			~StreamReaderOffsetBackup()
			{
				m_Reader.Seek(m_Offset);
			}
		private:
			StreamReader& m_Reader;
			size_t m_Offset;
		};

		void FillIndicesOffsets(StreamReader& Reader);

		std::pair<kstd::string, StructMembers> ParseStructure(StreamReader& Reader);

		void ParseFieldList(StreamReader& Reader, StructMembers& Members, size_t FieldListSize);

		template<typename T>
		std::pair<kstd::string, StructMembers> GetStructeWithMembers(T StructureLeaf, StreamReader& Reader)
		{
			if (!StructureLeaf.Count)
				return {};
			
			ReadNumeric(Reader);

			const auto name = Reader.ReadString();
			if (SkipStruct(name))
				return {};

			const auto [fieldListOffset, fieldListSize] = m_IndicesOffsets[StructureLeaf.Field - m_MinimumIndex];

			StreamReaderOffsetBackup offsetBackup(Reader);
			Reader.Seek(fieldListOffset);

			StructMembers members{};
			ParseFieldList(Reader, members, fieldListSize);

			return std::make_pair(name.data(), std::move(members));
		}

		std::optional<BitfieldData> GetBitfieldData(StreamReader& Reader, size_t Index);

		bool SkipStruct(std::string_view StructName);

		uint64_t ReadNumeric(StreamReader& Reader);

		kstd::vector<std::pair<size_t, size_t>> m_IndicesOffsets;
		size_t m_MinimumIndex;
		size_t m_MaximumIndex;
		MsfReader& m_MsfReader;
	};

}