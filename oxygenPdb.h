#pragma once
#include <fltKernel.h>


#include <fltKernel.h>


namespace oxygenPdb {

	class Pdber {

	public:
		Pdber(const wchar_t* moduleName);
		bool init();
		~Pdber();
		ULONG_PTR GetPointer(const char* name);
		size_t GetOffset(const char* structName, const char* propertyName);
	private:
		char padding[1000];//can not revise this!!! else it will ocurrs stack overflow!!
	};

}
