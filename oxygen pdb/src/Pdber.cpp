#include "Pdber.hpp"
#include <log.h>



namespace oxygenPdb {
	extern "C" wchar_t* __cdecl wcscat(wchar_t* Dest, const wchar_t* Source);
	extern "C" wchar_t* __cdecl wcscpy(wchar_t* Dest, const wchar_t* Source);
	extern "C" char* __cdecl strcpy(char* Dest, const char* Source);

	Pdber::Pdber(const wchar_t* moduleName) :_moduler(moduleName), _initfailed(false)
	{

	}

	bool Pdber::init()
	{
		const auto [base, size] = _moduler.getModuleInfo();
		do {
			if (base == 0 || size == 0) {
				break;
			}
			//viewer class to get pdb guid(md5 of pdb file and pdb name) 
			//move semantics will done when as a func return value
			auto info = _pdbViewer.getPdbInfo(base);


			//then download pdb or open pdb(pdb has exits
			const auto& pdb = _pdbViewer.downLoadPdb(info);
			// Check pdb is empty instead of nullptr
			if (pdb.empty()) {
				printk("Failed to download pdb\r\n");
				break;
			}
			
			const auto& cpath = pdb.data();
			if (cpath == 0) break;
			wcscpy(_pdbPath, cpath);
			strcpy(_pdbGuid, info.first.data());

			//init pdber file stream
			wchar_t wPdbFilePath[MAX_PATH]{ 0 };
			wcscpy(wPdbFilePath, L"\\??\\");
			wcscat(wPdbFilePath, _pdbPath);
			symbolic_access::FileStream pdbFileStream(wPdbFilePath);
			symbolic_access::MsfReader msfReader(std::move(pdbFileStream));
			if (!msfReader.Initialize())
			{
				printk("Failed to initialize msf reader %\r\n");
				break;
			}

			//symbol map ¾ÍÊÇÁ÷->·ûºÅµÄÓ³Éä
			symbolic_access::SymbolsExtractor symbolsExtractor(msfReader);
			_symbols = symbolsExtractor.Extract();
			if (_symbols.empty())
			{
				printk("Failed to extract symbols for %\r\n");
				break;
			}

			_structs = symbolic_access::StructExtractor(msfReader).Extract();
			return true;

		} while (0);

		//happens an error when executing there!
		printk("failed to open or download pdb file!\r\n");
		_initfailed = true;
		return false;
	}

	Pdber::~Pdber()
	{
		if (_initfailed) {
			return;
		}
		else {


			return;
		}


	}




}
