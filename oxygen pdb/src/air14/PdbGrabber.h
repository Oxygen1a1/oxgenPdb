#pragma once
#include <SymbolicAccess/Internal/string.h>
#include <SymbolicAccess/Internal/memory.h>
#include <SymbolicAccess/Utils/ProcessCreatorInterface.h>
#include <SymbolicAccess/Utils/FileStream.h>
#include <optional>

namespace symbolic_access
{
	class PdbGrabber
	{
	public:
		PdbGrabber(internal::unique_ptr<ProcessCreatorInterface>&& ProcessCreator);
		PdbGrabber(const PdbGrabber&) = delete;
		PdbGrabber& operator=(const PdbGrabber&) = delete;
		PdbGrabber(PdbGrabber&&) = default;
		PdbGrabber& operator=(PdbGrabber&&) = default;

		std::optional<FileStream> GetPdbFileStream(size_t ModuleBaseAddress);
	private:
		std::pair<internal::wstring, internal::wstring> GetPdbNameAndGuidPlusAge(size_t ImageDosHeaderAddress);

		internal::wstring CreateDownloadCommand(std::wstring_view PdbName, std::wstring_view GuidPlusAge, std::wstring_view PdbFinalPath);

		internal::wstring CreatePdbFilePath(std::wstring_view PdbName, std::wstring_view GuidPlusAge);

		internal::unique_ptr<ProcessCreatorInterface> m_ProcessCreator;
		internal::wstring m_DllPath = LR"(C:\WINDOWS\system32\WindowsPowerShell\v1.0;C:\Windows\system32;C:\Windows\system;C:\Windows;%SystemRoot%\system32\WindowsPowerShell\v1.0\;C:\Windows\system32;C:\Windows;C:\Windows\System32\Wbem;C:\Windows\System32\WindowsPowerShell\v1.0\)";
		internal::wstring m_SymbolsDir = LR"(\??\C:\Symbols\)";
		internal::wstring m_CurrentDir = LR"(C:\)";
		internal::wstring m_PowershellPath = 
#ifdef _KERNEL_MODE
			LR"(\??\C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe)";
#else
			LR"(C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe)";
#endif
	};
}