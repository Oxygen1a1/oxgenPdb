#pragma once
#include <fltKernel.h>
#include <kstl/string.h>
#include <kstl/map.h>

namespace oxygenPdb {

	class Moduler {
		using ptr_t = UINT_PTR;
	public:
		Moduler(const wchar_t* moduleName);
		~Moduler()=default;
		std::pair<ptr_t, ULONG> getModuleInfo();
		ptr_t constexpr getModuleBase() { return _moduleBase; }
	private:
		std::pair<ptr_t,ULONG> findModule(kstd::wstring moduleName);//get module base and size
		ptr_t _moduleBase;
		ULONG _moduleSize;

	};

}