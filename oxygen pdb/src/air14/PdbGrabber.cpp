#include <SymbolicAccess/Phnt/phnt.h>
#include <SymbolicAccess/Pdb/PdbGrabber.h>
#include <SymbolicAccess/Utils/Registry.h>
#include <SymbolicAccess/Utils/Log.h>
#include <algorithm>

namespace symbolic_access
{
	struct PdbInfo
	{
		ULONG Signature;
		GUID Guid;
		ULONG Age;
		char PdbFileName[1];
	};

	PdbGrabber::PdbGrabber(internal::unique_ptr<ProcessCreatorInterface>&& ProcessCreator) : m_ProcessCreator(std::move(ProcessCreator))
	{
		const auto& systemRootData = GetRegistryData(LR"(\REGISTRY\MACHINE\SOFTWARE\Microsoft\Windows NT\CurrentVersion)", L"SystemRoot");
		if (systemRootData.empty())
			return;

		for (auto string : { &m_DllPath, &m_SymbolsDir, &m_CurrentDir, &m_PowershellPath })
			std::replace(string->begin(), string->end(), L'C', *reinterpret_cast<const wchar_t*>(&systemRootData[0]));
	}

	std::optional<FileStream> PdbGrabber::GetPdbFileStream(size_t ModuleBaseAddress)
	{
		__debugbreak();
		//获取PDB的文件名词 和GUID 这个GUID好像就是MD5
		const auto& [pdbName, guidPlusAge] = GetPdbNameAndGuidPlusAge(ModuleBaseAddress);
		if (pdbName.empty() || guidPlusAge.empty())
		{
			PrintDbg("PdbName or GuidPlusAge is empty\n");
			return {};
		}

		//其实就是拼接一下
		const auto& pdbPath = CreatePdbFilePath(pdbName, guidPlusAge);
		if (pdbPath.empty())
		{
			PrintDbg("PdbPath is empty\n");
			return {};
		}

		auto fileStream = FileStream(pdbPath);
		if (!fileStream)
		{
			if (!m_ProcessCreator)
			{
				PrintDbg("ProcessCreator is null and there is no pdb on a disk\n");
				return {};
			}

			//用PowerShell去下载
			const auto& [processHandle, threadHandle] = m_ProcessCreator->CreateUmProcess(
				m_PowershellPath,
				CreateDownloadCommand(pdbName, guidPlusAge, std::wstring_view{ pdbPath.data() + pdbPath.find_first_of(LR"(C)") }),
				m_CurrentDir,
				m_DllPath);

			if (!processHandle || !threadHandle)
				return {};

			processHandle.WaitFor();

			fileStream = FileStream(pdbPath);
			if (!fileStream)
			{
				PrintDbg("Couldn't create handle to pdb file %S\n", pdbPath.data());
				return {};
			}
		}

		return fileStream;
	}

	//解析PE头 解析GUID和PDB名词 返回std::pair 然后用结构化绑定去接受
	std::pair<internal::wstring, internal::wstring> PdbGrabber::GetPdbNameAndGuidPlusAge(size_t ImageDosHeaderAddress)
	{
		const auto imageDosHeader = reinterpret_cast<IMAGE_DOS_HEADER*>(ImageDosHeaderAddress);
		const auto ntHeaders = reinterpret_cast<IMAGE_NT_HEADERS*>(ImageDosHeaderAddress + imageDosHeader->e_lfanew);
		const auto imageDebugDir = reinterpret_cast<IMAGE_DEBUG_DIRECTORY*>(
			ImageDosHeaderAddress + ntHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_DEBUG].VirtualAddress);

		//通过获取DEBUG节区的信息
		const auto pdbInfo = reinterpret_cast<PdbInfo*>(ImageDosHeaderAddress + imageDebugDir->AddressOfRawData);
		internal::wstring guidPlusAgeString(40, 0);

		//反正就这样就能获取到PDB文件的MD5
		swprintf_s(guidPlusAgeString.data(), guidPlusAgeString.size(), L"%08X%04X%04X%02X%02X%02X%02X%02X%02X%02X%02X%x",
			pdbInfo->Guid.Data1, pdbInfo->Guid.Data2, pdbInfo->Guid.Data3,
			pdbInfo->Guid.Data4[0], pdbInfo->Guid.Data4[1], pdbInfo->Guid.Data4[2], pdbInfo->Guid.Data4[3],
			pdbInfo->Guid.Data4[4], pdbInfo->Guid.Data4[5], pdbInfo->Guid.Data4[6], pdbInfo->Guid.Data4[7],
			pdbInfo->Age);

		guidPlusAgeString.resize(wcslen(guidPlusAgeString.data()));


		//std::string_view是C++17标准库中引入的一种非拥有型字符串引用类型。
		//	它提供了一种引用字符串或者字符数组的轻量级方式，
		//	可以看作是一个指针和一个长度，指向一个已经存在的字符串或者字符数组，但并不拥有它。
		std::string_view pdbFileName{ pdbInfo->PdbFileName };
		return std::make_pair(internal::wstring(pdbFileName.begin(), pdbFileName.end()), std::move(guidPlusAgeString));
	}

	internal::wstring PdbGrabber::CreateDownloadCommand(std::wstring_view PdbName, std::wstring_view GuidPlusAge, std::wstring_view PdbFinalPath)
	{
		internal::wstring result{};

		result += L"powershell.exe ";
		result += L"mkdir ";
		result += std::wstring_view{ PdbFinalPath.data(), PdbFinalPath.find_last_of(LR"(\)") };
		result += L"; $cli = New-Object System.Net.WebClient; \
		$cli.Headers['User-Agent'] = 'Microsoft-Symbol-Server/10.0.10036.206';\
		$cli.DownloadFile('http://msdl.microsoft.com/download/symbols/";
		result += PdbName;
		result += L"/";
		result += GuidPlusAge;
		result += L"/";
		result += PdbName;
		result += L"', '";
		result += PdbFinalPath;
		result += L"');";

		return result;
	}

	internal::wstring PdbGrabber::CreatePdbFilePath(std::wstring_view PdbName, std::wstring_view GuidPlusAge)
	{
		internal::wstring path{};
		path += m_SymbolsDir;
		path += PdbName;
		path += LR"(\)";
		path += GuidPlusAge;
		path += LR"(\)";
		path += PdbName;
		return path;
	}
}