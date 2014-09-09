/****************************************************************************
  Copyright (c) 2013-2014 libo.
 
  losemymind.libo@gmail.com

****************************************************************************/

#include <sstream>
#include "DataStream.h"

namespace Foundation
{

DataStream::DataStream()
{
}

DataStream::DataStream(DataStream&& pDataStream): 
		m_buffer(std::move(pDataStream.m_buffer))
{
}

DataStream& DataStream::operator=(DataStream&& pDataStream)
{
	m_buffer = std::move(pDataStream.m_buffer);
	return *this;
}

DataStream& DataStream::operator<<(bool data)
{
	*this << uint8_t(data ? 1 : 0);
	return *this;
}

DataStream& DataStream::operator<<(char data)
{
	m_buffer.append((char*)&data, sizeof(char));
	return *this;
}

DataStream& DataStream::operator<<(const uint8_t& data)
{
	m_buffer.append((char*)&data, sizeof(uint8_t));
	return *this;
}

DataStream& DataStream::operator<<(const uint16_t& data)
{
	m_buffer.append((char*)&data, sizeof(uint16_t));
	return *this;
}

DataStream& DataStream::operator<<(const uint32_t& data)
{
	m_buffer.append((char*)&data, sizeof(uint32_t));
	return *this;
}

DataStream& DataStream::operator<<(const uint64_t& data)
{
	m_buffer.append((char*)&data, sizeof(uint64_t));
	return *this;
}

DataStream& DataStream::operator<<(const int8_t& data)
{
	m_buffer.append((char*)&data, sizeof(int8_t));
	return *this;
}

DataStream& DataStream::operator<<(const int16_t& data)
{
	m_buffer.append((char*)&data, sizeof(int16_t));
	return *this;
}

DataStream& DataStream::operator<<(const int32_t& data)
{
	m_buffer.append((char*)&data, sizeof(int32_t));
	return *this;
}

DataStream& DataStream::operator<<(const int64_t& data)
{
	m_buffer.append((char*)&data, sizeof(int64_t));
	return *this;
}

DataStream& DataStream::operator<<(const float& data)
{
	m_buffer.append((char*)&data, sizeof(float));
	return *this;
}

DataStream& DataStream::operator<<(const double& data)
{
	m_buffer.append((char*)&data, sizeof(double));
	return *this;
}

DataStream& DataStream::operator<<( const char* data )
{
	*this << std::string(data);
	return *this;
}

DataStream& DataStream::operator<<(const std::string& data)
{
	*this << data.size();
	m_buffer.append(data.c_str(), data.size());
	return *this;
}



DataStream& DataStream::operator>>(bool& data)
{
	uint8_t i;
	*this >> i;
	data = i ? true : false;
	return *this;
}

DataStream& DataStream::operator>>(char& data)
{
	read(data);
	return *this;
}

DataStream& DataStream::operator>>(uint8_t& data)
{
	read(data);
	return *this;
}

DataStream& DataStream::operator>>(uint16_t& data)
{
	read(data);
	return *this;
}

DataStream& DataStream::operator>>(uint32_t& data)
{
	read(data);
	return *this;
}

DataStream& DataStream::operator>>(uint64_t& data)
{
	read(data);
	return *this;
}

DataStream& DataStream::operator>>(int8_t& data)
{
	read(data);
	return *this;
}

DataStream& DataStream::operator>>(int16_t& data)
{
	read(data);
	return *this;
}

DataStream& DataStream::operator>>(int32_t& data)
{
	read(data);
	return *this;
}

DataStream& DataStream::operator>>(int64_t& data)
{
	read(data);
	return *this;
}

DataStream& DataStream::operator>>(float& data)
{
	read(data);
	return *this;
}

DataStream& DataStream::operator>>(double& data)
{
	read(data);
	return *this;
}

DataStream& DataStream::operator>>(std::string& data)
{
	uint32_t size = 0;
	*this >> size;

	// Check for fake string size to prevent memory hacks
	if(size > m_buffer.size())
	{
		std::ostringstream os;
		os << "String size (" << size << ") > packet size (" << m_buffer.size() << ")";
		throw std::out_of_range(os.str());
	}
	if(size == 0)
	{
		data = "";
		return *this;
	}

	data = m_buffer.substr(0,size);
	data.shrink_to_fit();
	m_buffer.erase(0,size);
	return *this;
}

void DataStream::write(uint8_t* data, size_t pSize, int32_t pPos)
{
	if(pPos < 0)
	{
		m_buffer.append(reinterpret_cast<char*>(data), pSize);
	}
	else if(pPos + pSize <= m_buffer.size())
	{
		std::memcpy(&m_buffer[pPos], data, pSize);
	}
}

void DataStream::read( uint8_t* data, size_t dataSize )
{
	std::memcpy(&data,&m_buffer[0],dataSize);
	m_buffer.erase(0,dataSize);
}

void DataStream::clear()
{
	m_buffer.clear();
}

void DataStream::reset(const std::string& data)
{
	m_buffer.clear();
	m_buffer.append(&data[0], data.size());
}

size_t DataStream::size()
{
	return (int32_t)m_buffer.size();
}

const std::string& DataStream::getBuffer()
{
	return m_buffer;
}

const char* DataStream::c_str()
{
	return m_buffer.c_str();
}


} // namespace Foundation