#pragma once
/*
Moduler is a product class in the Factory Pattern, 
and in combination with the PDB Viewer class, 
it can parse the symbols within the module.
*/
#include <fltKernel.h>
#include <kstl/memory.h>
#include "viewer.hpp"
#include <kstl/string.h>
#include "moduler.hpp"
#include "./air14/FileStream.h"
#include "./air14/MsfReader.h"
#include "./air14/SymbolExtractor.h"
#include "./air14/StructExtractor.h"
namespace oxygenPdb {


	class Pdber {
		
	public:
		Pdber(const wchar_t* moduleName);
		~Pdber();
		bool init();
		ULONG_PTR GetPointer(const char* name);
		size_t GetOffset(const char* structName, const char* propertyName);
	private:
		using ptr_t = UINT_PTR;
		PdbViewer _pdbViewer;//Responsible for downloading and parsing PDB.
		Moduler _moduler;//responsible for get module base..
		char _pdbGuid[100];
		//kstd::unique_ptr<unsigned char[]> _pdbBuf;
		wchar_t _pdbPath[MAX_PATH];
		symbolic_access::SymbolsMap _symbols;
		symbolic_access::StructsMap _structs;
		bool _initfailed;
	};



	ULONG_PTR Pdber::GetPointer(const char* name)
	{
		if (_initfailed) return 0;
		std::string_view SymbolName(name);

		const auto& iter = _symbols.find(SymbolName);
		if (iter == _symbols.end())
			return 0;

		return (ULONG_PTR)(_moduler.getModuleBase() + iter->second);
	}

	//Get offset
	size_t Pdber::GetOffset(const char* structName, const char* propertyName) {

		const auto& structsIter = this->_structs.find(structName);
		if (structsIter == _structs.end())
			return 0;

		const auto& membersIter = std::find_if(structsIter->second.begin(), structsIter->second.end(),
			[&](const auto& MemberNameAndOffset) { return MemberNameAndOffset.Name == propertyName; });

		if (membersIter == structsIter->second.end())
			return 0;

		return membersIter->Offset;
	}

}