#pragma once
#include <fltKernel.h>
#include "Pdber.hpp"


namespace oxygenPdb {
	class PdbWrapper {
	public:
		PdbWrapper(const wchar_t* name);
		~PdbWrapper();
		ULONG_PTR getPointer(const char* name);
	private:
		Pdber _pdb;

	};



}

