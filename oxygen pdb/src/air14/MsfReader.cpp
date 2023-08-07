#include "MsfReader.h"
#include <algorithm>

namespace symbolic_access
{
	MsfReader::MsfReader(FileStream FileStream) : m_FileStream(std::move(FileStream)), m_PageSize(0)
	{
	}

	//初始化 解析pdb文件
	bool MsfReader::Initialize()
	{
		//MsfHeader=Multi-Stream Format
		//这是pdb文件专属的头 类似DOS头
		//先获取DOS头 先判断魔数
		MsfHeader msfHeader{};
		if (!m_FileStream.Read(&msfHeader, sizeof(msfHeader)) || msfHeader.Signature != m_Signature)
			return false;

		//PDB文件按流解析 不同的流代表不同的数据 
		//有一些流包含了符号信息，一些流包含了类型信息
		//一个页的大小
		//流目录是一个表，它描述了流的位置和大小。 大小是以BLOCK(页)
		//都是按照这个对齐的

		m_PageSize = msfHeader.PageSize;

		//根目录页
		const auto numberOfRootPages = GetNumberOfPages(msfHeader.DirectorySize);
		if (!numberOfRootPages)
			return false;

		//存放索引需要几个block 这行代码试图确定需要多少索引页来描述所有的根目录页。
		//MSF 文件的目录是分页的，并且这些页通过索引页来描述

		const auto numberOfRootIndexPages = GetNumberOfPages(numberOfRootPages * sizeof(uint32_t));
		if (!numberOfRootIndexPages)
			return false;

		kstd::vector<uint32_t> rootPages(numberOfRootPages);
		kstd::vector<uint32_t> rootIndexPages(numberOfRootIndexPages);

		//这个时候文件流已经越过Msf Header了
		//也就是msf header紧接着是描述根目录页的索引的信息
		//同时|| 检查rootIndexPages是否有0 有0就返回
		//这样九八rootIndexPages填满了?
		//pdb文件的结构
		/*
			MSF头
			numberOfRootIndexPages的uint32 指向的是一个pages的索引
		*/
		if (!m_FileStream.Read(rootIndexPages.data(), numberOfRootIndexPages * sizeof(uint32_t)) || HasNullOffsets(rootIndexPages))
			return false;

		//跟目录索引页有多少个
	
		for (uint32_t i{}, k{}, len{}; i < numberOfRootIndexPages; ++i, k += len)
		{
			//索引页里面肯定都是索引 一个都是4个字节
			//前者是一个页里面有多少索引 
			len = min(m_PageSize / 4, numberOfRootPages - k);//也就是一次最多处理一页
			//文件的指针到指定位置 也就是跑到索引页  如果超过 那就得找另一个索引
			m_FileStream.Seekg(rootIndexPages[i] * m_PageSize);
			if (!m_FileStream.Read(&rootPages[k], len * 4))//读取索引页 跟页的索引
				return false;
		}

		if (HasNullOffsets(rootPages))
			return false;

		uint32_t numberOfStreams{};//获取流的数量
		m_FileStream.Seekg(rootPages[0] * m_PageSize);//第0个根页的索引指向的块(页)的前32位是流数量
		if (!m_FileStream.Read(&numberOfStreams, sizeof(numberOfStreams)))
			return false;
		m_Streams.reserve(numberOfStreams);//m_Streams是一个vector 结构是一个流的结构
		//流结构是 4字节大小 后面跟一个变长数组
		uint32_t currentRootPage{};
		for (uint32_t i{}; i < numberOfRootPages; ++i)
		{
			if(i)//seek到第i个根页的索引
				m_FileStream.Seekg(rootPages[i] * m_PageSize);
			//如果i==0 k=1,否则k=0 也就是说只有根页索引是第一个的时候 k跳过,因为第零个根页索引指向的页前4字节是流的个数
			for (uint32_t k = i == 0; numberOfStreams > 0 && k < m_PageSize / 4; ++k, --numberOfStreams)
			{//最多只能每次处理一个页 
				uint32_t size{};
				if (!m_FileStream.Read(&size, sizeof(size)))//参见stream的结构 只有根页索引是0指向的才从0x4开始
					return false;
				//那些RootPages其实是流大小组成的 这里可以看到 RootPages的前几个其实就是流大小的索引
				if (size == 0xFFFFFFFF)
					size = 0;
				//后面就是流的内容了?
				m_Streams.emplace_back(ContentStream{ size });//填充到流信息
			}

			if (!numberOfStreams)
			{
				currentRootPage = i;
				break;
			}
		}

		for (auto& stream : m_Streams)
		{
			const auto numberOfPages = GetNumberOfPages(stream.Size);
			if (!numberOfPages)
				continue;

			stream.PageIndices.resize(numberOfPages);//获取流后面跟着的索引 这是在申请空间

			for (auto remainingPages = numberOfPages; remainingPages > 0;)
			{	//按照页大小对齐偏移 也就是读完那些流的大小 直接紧接着从读的地方继续开始
				const auto pageOffset = static_cast<uint32_t>(m_FileStream.Tellg()) % m_PageSize;
				const auto pageSize = min(remainingPages * 4, m_PageSize - pageOffset);//不能读跨页(按照页)
				//读取 也就是说 紧接着后面就是这些流的索引
				if (!m_FileStream.Read(stream.PageIndices.data() + numberOfPages - remainingPages, pageSize))
					return false;
				//减去读到的
				remainingPages -= pageSize / 4;

				if (pageOffset + pageSize == m_PageSize)//如果是这个 流的数量存的索引超过一个页
					m_FileStream.Seekg(rootPages[++currentRootPage] * m_PageSize);//要+1 读取下一个Pages 如此反复 
			}//就可以读完全部的流大小+流后面跟着的索引
		}

		return true;
	}

	kstd::vector<char> MsfReader::GetStream(size_t Index)
	{
		const auto& stream = m_Streams[Index];
		kstd::vector<char> streamData(stream.PageIndices.size() * m_PageSize);

		size_t offset{};
		for (const auto pageIndex : stream.PageIndices)
		{
			m_FileStream.Seekg(pageIndex * m_PageSize);
			m_FileStream.Read(streamData.data() + offset, m_PageSize);

			offset += m_PageSize;
		}

		streamData.resize(stream.Size);
		return streamData;
	}

	//就是DirectorySize/一页的大小 然后+1或者不加
	uint32_t MsfReader::GetNumberOfPages(uint32_t Size)
	{
		const auto numberOfPages = Size / m_PageSize;
		return Size % m_PageSize ? numberOfPages + 1 : numberOfPages;
	}

	//检查是否整个容器里面是否有0
	bool MsfReader::HasNullOffsets(const kstd::vector<uint32_t>& Offsets)
	{
		return std::any_of(Offsets.begin(), Offsets.end(), [](uint32_t Offset) { return Offset == 0; });
	}
}