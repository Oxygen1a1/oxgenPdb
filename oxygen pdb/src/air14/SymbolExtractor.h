#pragma once
#include "MsfReader.h"
#include "PdbStructs.h"
#include "ModuleData.h"
#include "StreamReader.h"
#include <charconv>

namespace symbolic_access
{
	class SymbolsExtractor
	{
	public:
		SymbolsExtractor(MsfReader& MsfReader);
		SymbolsExtractor(const SymbolsExtractor&) = delete;
		SymbolsExtractor& operator=(const SymbolsExtractor&) = delete;
		SymbolsExtractor(SymbolsExtractor&&) = default;
		SymbolsExtractor& operator=(SymbolsExtractor&&) = default;

		SymbolsMap Extract();
	private:
		struct IMAGE_SECTION_HEADER
		{
			uint8_t Name[8];
			union
			{
				uint32_t PhysicalAddress;
				uint32_t VirtualSize;
			} Misc;
			uint32_t VirtualAddress;
			uint32_t SizeOfRawData;
			uint32_t PointerToRawData;
			uint32_t PointerToRelocations;
			uint32_t PointerToLinenumbers;
			uint16_t NumberOfRelocations;
			uint16_t NumberOfLinenumbers;
			uint32_t Characteristics;
		};

		kstd::vector<IMAGE_SECTION_HEADER> ReadSectionHeaders(uint32_t HeaderStream);

		kstd::vector<DbiModuleInfo> GetModulesInfo(StreamReader& Reader, size_t ModulesSize);

		bool GetGlobalSymbols(uint16_t SymbolRecordStreamIndex, SymbolsMap& Symbols);

		bool GetModuleSymbols(const DbiModuleInfo& ModuleInfo, SymbolsMap& Symbols);

		void GetSymbol(StreamReader& Reader, SymbolsMap& Symbols);

		bool SkipSymbol(std::string_view SymbolName);

		void ReadOmaps(uint16_t OmapFromSourceStreamIndex, uint16_t OmapToSourceStreamIndex);

		std::optional<size_t> GetSymbolsOffset(size_t Offset, uint16_t Section);

		std::optional<size_t> TranslateOffset(size_t Offset, const kstd::vector<OMAP>& Omaps);

		template<typename T>
		void InsertSymbol(StreamReader& Reader, SymbolsMap& Symbols)
		{
			const auto symbol = Reader.Read<T>();
			const auto name = Reader.ReadString();
			if (SkipSymbol(name))
				return;

			const auto offset = GetSymbolsOffset(symbol.Off, symbol.Seg);
			if (!offset.has_value())
				return;

			kstd::string nameCopy = name.data();
			auto symbolIter = Symbols.find(nameCopy);
			for (size_t i{}; symbolIter != Symbols.end() && symbolIter->second != *offset; ++i, symbolIter = Symbols.find(nameCopy))
			{
				std::array<char, 21> buffer{};
				std::to_chars(buffer.data(), buffer.data() + buffer.size(), i);
				nameCopy = name;
				nameCopy += "_";
				nameCopy += buffer.data();
			}

			Symbols.insert(std::make_pair(std::move(nameCopy), *offset));
		}

		MsfReader& m_MsfReader;
		kstd::vector<IMAGE_SECTION_HEADER> m_SectionHeaders;
		kstd::vector<IMAGE_SECTION_HEADER> m_OriginalSectionHeaders;
		kstd::vector<OMAP> m_FirstOmaps;
		kstd::vector<OMAP> m_SecondOmaps;
	};
}