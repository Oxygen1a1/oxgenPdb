#include "PdbWrapper.h"

namespace oxygenPdb {



	PdbWrapper::PdbWrapper(const wchar_t* name):_pdb(name)
	{
	}

	PdbWrapper::~PdbWrapper()
	{
		_pdb.~Pdber();
	}

	ULONG_PTR PdbWrapper::getPointer(const char* name)
	{
		return (UINT_PTR)_pdb.GetPointer<ULONG_PTR>(name);
	}

}