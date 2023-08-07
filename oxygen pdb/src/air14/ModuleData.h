#pragma once
#include <utility>
#include <optional>
#include <kstl/map.h>
#include <kstl/string.h>
#include <kstl/vector.h>

namespace symbolic_access
{
	struct BitfieldData
	{
		uint8_t Position;
		uint8_t Length;
	};

	struct Member
	{
		kstd::string Name;
		size_t Offset;
		std::optional<BitfieldData> Bitfield;
	};

	using StructMembers = kstd::vector<Member>;
	using StructsMap = kstd::map<kstd::string, StructMembers, std::less<>>;
	using SymbolsMap = kstd::map<kstd::string, size_t, std::less<>>;
}