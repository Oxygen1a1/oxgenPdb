#include "viewer.hpp"
#include <ntimage.h>
#include <ksocket/sockets.h>
#include <log.h>
#include <ntstrsafe.h>

namespace oxygenPdb{
	//contract with crt...
	EXTERN_C int sprintf_s(
		char* buffer,
		size_t sizeOfBuffer,
		const char* format,
		...
	);
	EXTERN_C int swprintf(wchar_t* String,
		size_t Count, 
		const wchar_t* Format, ...);

	const char* downUrl = "http://msdl.blackint3.com/download/symbols/";
	struct PdbInfo
	{
		ULONG Signature;
		GUID Guid;
		ULONG Age;
		char PdbFileName[1];
	};

	using ptr_t = UINT_PTR;
	using pdbInfo_t = std::pair<kstd::string, kstd::string>;

	//give an module base it will return it's pdb guid and pdb name(
	pdbInfo_t PdbViewer::getPdbInfo(ptr_t mBase)
	{
		const auto dosHeader = reinterpret_cast<IMAGE_DOS_HEADER*>(mBase);
		const auto ntHeader = reinterpret_cast<IMAGE_NT_HEADERS*>(mBase + dosHeader->e_lfanew);
		//get debug dir
		const auto dbgDir = reinterpret_cast<IMAGE_DEBUG_DIRECTORY*>(mBase +
			ntHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_DEBUG].VirtualAddress);
		const auto pdbInfo= reinterpret_cast<PdbInfo*>(mBase + dbgDir->AddressOfRawData);

		//get guid(md5) md5 size of 32
		kstd::string pdbGuid(40, 0);

		sprintf_s(pdbGuid.data(), pdbGuid.size(), "%08x%04x%04x%02x%02x%02x%02x%02x%02x%02x%02x%x",
			pdbInfo->Guid.Data1, pdbInfo->Guid.Data2, pdbInfo->Guid.Data3,
			pdbInfo->Guid.Data4[0], pdbInfo->Guid.Data4[1], pdbInfo->Guid.Data4[2], pdbInfo->Guid.Data4[3],
			pdbInfo->Guid.Data4[4], pdbInfo->Guid.Data4[5], pdbInfo->Guid.Data4[6], pdbInfo->Guid.Data4[7],
			pdbInfo->Age);
		pdbGuid.resize(strlen(pdbGuid.data()));
		//get pdb name
		kstd::string pdbName(pdbInfo->PdbFileName);
		

		return std::pair(pdbGuid,pdbName);
	}

	//if pdb file exits return pdb file path
	//else download and then retrun file path
	kstd::wstring PdbViewer::downLoadPdb(pdbInfo_t& info)
	{
		wchar_t wPdbName[MAX_PATH]{ 0 };
		auto wRetPdbName = ansiToUni(wPdbName, MAX_PATH, info.second.c_str());
		//first check if the pdb has been downloaded over here
		kstd::wstring _pdbPath = kstd::wstring(L"C:\\Windows\\Temp\\") + wRetPdbName;
		kstd::string pdbPath = "C:\\Windows\\Temp\\" + info.second;
		auto fileExits = isFileExits(pdbPath);
		if (fileExits) {
			//exits so we create file and get it's content
			return _pdbPath;

		}

		//get url
		kstd::string url = downUrl+info.second+"/"+info.first+"/"+info.second;
		printk("download url:%s",url.c_str());

		//must init else bsod
		ksocket::init();
		auto pdbSize=ksocket::getContentLength(url.c_str(), "88");
		if (pdbSize == 0) {
			printk("failed to get content length!\r\n");
			return nullptr;
		}

		auto pdbBuf = kstd::make_unique<unsigned char[]>(pdbSize + 500);
		auto bSuc=ksocket::getHttpContent(url.c_str(), (char*)pdbBuf.get(), pdbSize + 500, "88");
		if (!bSuc) {

			printk("failed to get pdb file!\r\n");
			return nullptr;
		}
		ksocket::destory();

		//then we write to disk
		if (wRetPdbName != wPdbName) {
			printk("failed to convert ansi to uni!\r\n");
			return nullptr;
		}
		helper::writeToDisk(L"C:\\Windows\\Temp\\", wRetPdbName,(char*)pdbBuf.get(), pdbSize);
		
		//not move semantics it's will auto
		return _pdbPath;
	}

	bool PdbViewer::isFileExits(kstd::string path)
	{
		ANSI_STRING  asPath{ 0 };
		UNICODE_STRING usPath{ 0 };
		OBJECT_ATTRIBUTES oa{ 0 };
		HANDLE hFile{ 0 };
		IO_STATUS_BLOCK iob{ 0 };
		NTSTATUS status = STATUS_UNSUCCESSFUL;
		kstd::string r0Path = "\\??\\" + path;

		RtlInitAnsiString(&asPath, r0Path.c_str());
		if (NT_SUCCESS(status=RtlAnsiStringToUnicodeString(&usPath,&asPath, true))) {
	
			InitializeObjectAttributes(&oa, &usPath, 
				OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE, NULL, NULL);

			status = ZwOpenFile(&hFile,
				SYNCHRONIZE, &oa,
				&iob,
				FILE_SHARE_READ, FILE_SYNCHRONOUS_IO_NONALERT);
			// Don't forget to free the memory if AllocateDestinationString was set to TRUE.
			RtlFreeUnicodeString(&usPath);
		}

		
		if (NT_SUCCESS(status)) {
			ZwClose(hFile);
			return true; //exits
		}
		return false;//no exits
	}

	kstd::unique_ptr<unsigned char[]> PdbViewer::getFileBuf(kstd::string path)
	{
		HANDLE file_handle{ 0 };
		OBJECT_ATTRIBUTES obj_attr{ 0 };
		IO_STATUS_BLOCK block{ 0 };
		LARGE_INTEGER offset{ 0 };
		FILE_STANDARD_INFORMATION info{ 0 };
		NTSTATUS status{ 0 };
		UNICODE_STRING uName{ 0 };
		ANSI_STRING  aName{ 0 };
		kstd::string r0Path;

		r0Path = "\\??\\"+path;
		RtlInitAnsiString(&aName, r0Path.c_str());
		//ansi ->unicode
		if (NT_SUCCESS(status = RtlAnsiStringToUnicodeString(&uName, &aName, true))) {
			
			InitializeObjectAttributes(&obj_attr, &uName, 0x40, 0, 0);

			status = ZwCreateFile(&file_handle, GENERIC_WRITE | GENERIC_READ, &obj_attr, &block, &offset, FILE_ATTRIBUTE_NORMAL, FILE_SHARE_READ, FILE_OPEN, 0, 0, 0);
			if (!NT_SUCCESS(status)) {

				printk("failed to create file!  errcode->%x\r\n", status);
				return nullptr;
			}

			status = ZwQueryInformationFile(file_handle, &block, &info, sizeof(info), FileStandardInformation);
			auto size = (ULONG)info.AllocationSize.QuadPart;

			if (!NT_SUCCESS(status)) {

				printk("failed to get file size  errcode->%x\r\n", status);
				return nullptr;
			}

			auto read_buf = kstd::make_unique<unsigned char[]>(size+100);
			if (read_buf == nullptr) {

				printk("failed to allocate memory! errcode->%x\r\n",status);
				NtClose(file_handle);
				return nullptr;
			}

			status = ZwReadFile(file_handle, 0, 0, 0, &block, read_buf.get(), size, &offset, 0);
			if (!NT_SUCCESS(status)) {

				ZwClose(file_handle);
				printk("failed to read file content errcode->%x",status);
				return nullptr;
			}

			//flush to avoid to wait!
			ZwFlushBuffersFile(file_handle, &block);
			ZwClose(file_handle);
			// Don't forget to free the memory if AllocateDestinationString was set to TRUE.
			RtlFreeUnicodeString(&uName);
			return read_buf;
		}

		printk("failed to convert ansi to unicode!\r\n");
		return nullptr;
		
	}

	//if success return dest which same as arg dest
	wchar_t* PdbViewer::ansiToUni(wchar_t* dest,ULONG size, const char* src)
	{
		UNICODE_STRING uDest{ 0 };
		ANSI_STRING aSrc{ 0 };
		//must has memory as container
		if (dest == nullptr) return 0;

		uDest.Buffer = dest;
		uDest.Length = 0;
		uDest.MaximumLength =(USHORT)((size * sizeof wchar_t));
		RtlInitAnsiString(&aSrc, src);
		if (NT_SUCCESS(RtlAnsiStringToUnicodeString(&uDest, &aSrc, false))) {
			return dest;
		}
		else return 0;

	}

}