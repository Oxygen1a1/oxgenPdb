#include "StreamReader.h"

namespace symbolic_access
{
	StreamReader::StreamReader(const kstd::vector<char>& Stream) : m_Stream(Stream), m_StreamOffset(0)
	{
	}

	void StreamReader::Skip(size_t Size)
	{
		m_StreamOffset += Size;
	}

	size_t StreamReader::Tellg()
	{
		return m_StreamOffset;
	}

	void StreamReader::Seek(size_t Offset)
	{
		m_StreamOffset = Offset;
	}

	void StreamReader::Align(size_t Align)
	{
		if (m_StreamOffset % Align != 0)
			Skip(Align - m_StreamOffset % Align);
	}

	size_t StreamReader::Read(void* Buffer, size_t Size)
	{
		if (m_StreamOffset >= m_Stream.size())
			return 0;

		Size = min(m_Stream.size() - m_StreamOffset, Size);
		std::memcpy(Buffer, m_Stream.data() + m_StreamOffset, Size);
		m_StreamOffset += Size;

		return Size;
	}

	std::string_view StreamReader::ReadString()
	{
		std::string_view result(m_Stream.data() + m_StreamOffset);
		m_StreamOffset += result.size() + 1;
		return result;
	}
}