#pragma once
#include <kstl/memory.h>
#include <map>
#include <optional>
#include <kstl/string.h>
namespace oxygenPdb {
	const ULONG MAX_PATH = 266;
	class PdbViewer {
		using ptr_t = UINT_PTR;
		using pdbInfo_t = std::pair<kstd::string, kstd::string>;
	public:
		PdbViewer() = default;
		~PdbViewer() = default;
		// get pdb guid(pdb md5 and pdb name to download)
		pdbInfo_t getPdbInfo(ptr_t
			mBase);
		kstd::wstring
			downLoadPdb(pdbInfo_t& info);
	private:
		
		bool isFileExits(kstd::string path);
		kstd::unique_ptr<unsigned char[]>
			getFileBuf(kstd::string path);
		wchar_t* ansiToUni(wchar_t* dest,ULONG size, const char* src);
	};

}