#pragma once
#include <kstl/vector.h>
#include <string_view>

namespace symbolic_access
{
	class StreamReader
	{
	public:
		StreamReader(const kstd::vector<char>& Stream);

		void Skip(size_t Size);

		size_t Tellg();

		void Seek(size_t Offset);

		void Align(size_t Align);

		template <typename T>
		T Read()
		{
			m_StreamOffset += sizeof(T);
			return *reinterpret_cast<const T*>(m_Stream.data() + m_StreamOffset - sizeof(T));
		}

		template <typename T>
		T Peek()
		{
			return *reinterpret_cast<const T*>(m_Stream.data() + m_StreamOffset);
		}

		size_t Read(void* Buffer, size_t Size);

		std::string_view ReadString();

	private:
		size_t m_StreamOffset;
		const kstd::vector<char>& m_Stream;
	};
}