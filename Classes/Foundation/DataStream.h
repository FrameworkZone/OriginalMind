/****************************************************************************
  Copyright (c) 2013-2014 libo.
 
  losemymind.libo@gmail.com

****************************************************************************/

#ifndef Foundation_DataStream1_h
#define Foundation_DataStream1_h

#include <cstdint>
#include <vector>
#include <list>
#include <map>
#include <unordered_map>
#include <string>

namespace Foundation
{
class  DataStream
{
public:
	DataStream();
	DataStream(DataStream&& pDataStream);
	DataStream& operator=(DataStream&& pDataStream);
	DataStream& operator<<(bool data);
	DataStream& operator<<(char data);
	DataStream& operator<<(const uint8_t& data);
	DataStream& operator<<(const uint16_t& data);
	DataStream& operator<<(const uint32_t& data);
	DataStream& operator<<(const uint64_t& data);
	DataStream& operator<<(const int8_t& data);
	DataStream& operator<<(const int16_t& data);
	DataStream& operator<<(const int32_t& data);
	DataStream& operator<<(const int64_t& data);
	DataStream& operator<<(const float& data);
	DataStream& operator<<(const double& data);
	DataStream& operator<<(const char* data);
	DataStream& operator<<(const std::string& data);

	DataStream& operator>>(char& data);
	DataStream& operator>>(bool& data);
	DataStream& operator>>(uint8_t& data);
	DataStream& operator>>(uint16_t& data);
	DataStream& operator>>(uint32_t& data);
	DataStream& operator>>(uint64_t& data);
	DataStream& operator>>(int8_t& data);
	DataStream& operator>>(int16_t& data);
	DataStream& operator>>(int32_t& data);
	DataStream& operator>>(int64_t& data);
	DataStream& operator>>(float& data);
	DataStream& operator>>(double& data);
	DataStream& operator>>(std::string& data);

	template <typename K, typename V>
	DataStream& operator<<(const std::map<K, V>& data)
	{
		return writeAssociativeContainer(data);
	}

	template <typename K, typename V>
	DataStream& operator>>(std::map<K, V>& data)
	{
		return readAssociativeContainer<std::map<K, V>, K, V>(data);
	}

	template <typename K, typename V>
	DataStream& operator<<(const std::unordered_map<K, V>& data)
	{
		return writeAssociativeContainer(data);
	}

	template <typename K, typename V>
	DataStream& operator>>(std::unordered_map<K, V>& data)
	{
		return readAssociativeContainer<std::unordered_map<K, V>, K, V>(data);
	}

	template <typename V>
	DataStream& operator<<(const std::vector<V>& data)
	{
		return writeSequenceContainer(data);
	}

	template <typename V>
	DataStream& operator>>(std::vector<V>& data)
	{
		return readSequenceContainer<std::vector<V>, V>(data);
	}

	template <typename V>
	DataStream& operator<<(const std::list<V>& data)
	{
		return writeSequenceContainer(data);
	}

	template <typename V>
	DataStream& operator>>(std::list<V>& data)
	{
		return readSequenceContainer<std::list<V>, V>(data);
	}

	void write			(uint8_t* data, size_t pSize, int32_t pPos = -1);

	template< typename T >
	void read(T& data)
	{
		if(m_buffer.size() < sizeof(T))
		{
			data = 0;
			return;
		}
		std::memcpy(&data,&m_buffer[0],sizeof(T));
		m_buffer.erase(0,sizeof(T));
	}

	template< typename T >
	inline T read()
	{
		T ret;
		read(ret);
		return ret;
	}

	void read(uint8_t* data, size_t dataSize);


	void   clear();
	void   reset(const std::string& data);
	size_t size();

	const char* c_str();

	const std::string&	getBuffer();

private:
	template<typename C>
	DataStream& writeSequenceContainer(const C& data)
	{
		*this << data.size();
		for(auto iter : data)
		{
			*this << iter;
		}
		return *this;
	}

	template<typename C, typename V>
	DataStream& readSequenceContainer(C& data)
	{
		uint32_t size;
		*this >> size;
		//if(size > 1000)
		//    return *this;

		for(uint64_t i = 0; i < size; ++i)
		{
			V value;
			*this >> value;
			data.push_back(value);
		}

		return *this;
	}

	template<typename C>
	DataStream& writeAssociativeContainer(const C& data)
	{
		*this << data.size();
		for(auto& iter : data)
		{
			*this << iter.first << iter.second;
		}
		return *this;
	}

	template<typename C ,typename K, typename V>
	DataStream& readAssociativeContainer(C& data)
	{
		uint32_t size;
		*this >> size;
		//if(size > 1000)
		//    return *this;

		for(uint64_t i = 0; i < size; ++i)
		{
			K key;
			V value;
			*this >> key >> value;
			data.insert(std::pair<K,V>(key, value));
		}
		return *this;
	}

protected:
	std::string		      m_buffer;
};

} // namespace Foundation
#endif // Foundation_DataStream_h