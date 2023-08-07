#include "StructExtractor.h"

namespace symbolic_access
{
	StructExtractor::StructExtractor(MsfReader& MsfReader) : m_MsfReader(MsfReader)
	{
	}

	StructsMap StructExtractor::Extract()
	{
		const auto& typeInfoStream = m_MsfReader.GetStream(MsfReader::KnownStreams::TypeInfoStream);
		if (typeInfoStream.empty())
			return {};

		StreamReader reader(typeInfoStream);

		const auto tpiHeader = reader.Read<TpiHeader>();
		m_MinimumIndex = tpiHeader.MinimumIndex;
		m_MaximumIndex = tpiHeader.MaximumIndex;

		FillIndicesOffsets(reader);

		StructsMap structs{};
		for (const auto [indexOffset, leafSize] : m_IndicesOffsets)
		{
			reader.Seek(indexOffset);

			auto [structName, structMembers] = ParseStructure(reader);
			if (structName.empty() || structMembers.empty())
				continue;

			if (const auto iter = structs.find(structName); iter != structs.end() && structMembers.size() > iter->second.size())
				iter->second = std::move(structMembers);
			else
				structs.insert({ std::move(structName), std::move(structMembers) });
		}

		return structs;
	}

	void StructExtractor::FillIndicesOffsets(StreamReader& Reader)
	{
		m_IndicesOffsets.resize(m_MaximumIndex - m_MinimumIndex);
		for (auto i = m_MinimumIndex; i < m_MaximumIndex; ++i)
		{
			const auto size = Reader.Read<uint16_t>();
			m_IndicesOffsets[i - m_MinimumIndex] = std::make_pair(Reader.Tellg(), size);
			Reader.Skip(size);
		}
	}

	std::pair<kstd::string, StructMembers> StructExtractor::ParseStructure(StreamReader& Reader)
	{
		switch (Reader.Read<Leaf>())
		{
		case Leaf::LF_STRUCTURE:
		case Leaf::LF_STRUCTURE_ST:
		case Leaf::LF_CLASS:
		case Leaf::LF_CLASS_ST:
		case Leaf::LF_INTERFACE:
			return GetStructeWithMembers(Reader.Read<LfClass>(), Reader);
		case Leaf::LF_STRUCTURE32:
			return GetStructeWithMembers(Reader.Read<LfClass32>(), Reader);
		case Leaf::LF_UNION:
		case Leaf::LF_UNION_ST:
			return GetStructeWithMembers(Reader.Read<LfUnion>(), Reader);
		}

		return {};
	}

	void StructExtractor::ParseFieldList(StreamReader& Reader, StructMembers& Members, size_t FieldListSize)
	{
		for (const auto finalOffset = Reader.Tellg() + FieldListSize; Reader.Tellg() < finalOffset;)
		{
			switch (Reader.Read<Leaf>())
			{
			case Leaf::LF_MEMBER:
			case Leaf::LF_MEMBER_ST:
			{
				const auto member = Reader.Read<LfMember>();
				const auto offset = ReadNumeric(Reader);
				const auto memberName = Reader.ReadString();
				Reader.Align(sizeof(uint32_t));
				Members.emplace_back(Member{ memberName.data(), static_cast<size_t>(offset), GetBitfieldData(Reader, member.Index) });
				break;
			}
			case Leaf::LF_INDEX:
			{
				const auto [fieldListOffset, fieldListSize] = m_IndicesOffsets[Reader.Read<LfIndex>().Index - m_MinimumIndex];

				StreamReaderOffsetBackup offsetBackup(Reader);
				Reader.Seek(fieldListOffset);
				ParseFieldList(Reader, Members, fieldListSize);
				break;
			}
			case Leaf::LF_VBCLASS:
			case Leaf::LF_IVBCLASS:
				Reader.Skip(sizeof(LfVbClass));
				ReadNumeric(Reader);
				ReadNumeric(Reader);
				Reader.Align(sizeof(uint32_t));
				break;
			case Leaf::LF_BCLASS:
				Reader.Skip(sizeof(LfBClass));
				ReadNumeric(Reader);
				Reader.Align(sizeof(uint32_t));
				break;
			case Leaf::LF_ENUMERATE:
			case Leaf::LF_ENUMERATE_ST:
				Reader.Skip(sizeof(LfEnumerate));
				Reader.ReadString();
				Reader.Align(sizeof(uint32_t));
				break;
			case Leaf::LF_STMEMBER:
			case Leaf::LF_STMEMBER_ST:
				Reader.Skip(sizeof(LfStMember));
				Reader.ReadString();
				Reader.Align(sizeof(uint32_t));
				break;
			case Leaf::LF_METHOD:
			case Leaf::LF_METHOD_ST:
				Reader.Skip(sizeof(LfMethod));
				Reader.ReadString();
				Reader.Align(sizeof(uint32_t));
				break;
			case Leaf::LF_ONEMETHOD:
			case Leaf::LF_ONEMETHOD_ST:
				switch (static_cast<MethodProperty>(Reader.Peek<LfOneMethod>().Attr.Mprop))
				{
				case MethodProperty::Intro:
				case MethodProperty::PureIntro:
					Reader.Skip(sizeof(uint32_t));
				}
				Reader.Skip(sizeof(LfOneMethod));
				Reader.ReadString();
				Reader.Align(sizeof(uint32_t));
				break;
			case Leaf::LF_NESTTYPE:
			case Leaf::LF_NESTTYPE_ST:
			case Leaf::LF_NESTTYPEEX:
			case Leaf::LF_NESTTYPEEX_ST:
				Reader.Skip(sizeof(LfNestTypeEx));
				Reader.ReadString();
				Reader.Align(sizeof(uint32_t));
				break;
			case Leaf::LF_VFUNCTAB:
				Reader.Skip(sizeof(LfVFuncTab));
				Reader.Align(sizeof(uint32_t));
				break;
			}

			while (Reader.Peek<uint8_t>() >= 0xf0)
			{
				const auto padding = Reader.Read<uint8_t>();
				if (const auto paddingSize = (padding & 0x0f) - 1; padding > 0xf0 && paddingSize)
					Reader.Skip(paddingSize);
			}
		}
	}

	std::optional<BitfieldData> StructExtractor::GetBitfieldData(StreamReader& Reader, size_t Index)
	{
		if (Index < m_MinimumIndex)
			return {};

		StreamReaderOffsetBackup offsetBackup(Reader);
		Reader.Seek(m_IndicesOffsets[Index - m_MinimumIndex].first);
		if (Reader.Read<Leaf>() != Leaf::LF_BITFIELD)
			return {};

		const auto bitfield = Reader.Read<LfBitfield>();
		return BitfieldData{ bitfield.Position, bitfield.Length };
	}

	bool StructExtractor::SkipStruct(std::string_view StructName)
	{
		return StructName.find("<anonymous-tag>") != std::string_view::npos ||
			StructName.find("<unnamed-tag>") != std::string_view::npos ||
			StructName.find("__unnamed") != std::string_view::npos ||
			StructName.find("<unnamed-type-u>") != std::string_view::npos;
	}

	uint64_t StructExtractor::ReadNumeric(StreamReader& Reader)
	{
		const auto leaf = Reader.Read<Leaf>();

		switch (leaf)
		{
		case Leaf::LF_CHAR: return Reader.Read<uint8_t>();
		case Leaf::LF_SHORT:
		case Leaf::LF_USHORT: return Reader.Read<uint16_t>();
		case Leaf::LF_LONG:
		case Leaf::LF_ULONG: return Reader.Read<uint32_t>();
		case Leaf::LF_QUADWORD:
		case Leaf::LF_UQUADWORD: return Reader.Read<uint64_t>();
		}

		return static_cast<uint64_t>(leaf);
	}
}