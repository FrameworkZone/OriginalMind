/****************************************************************************
  Copyright (c) 2013-2014 libo.
 
  losemymind.libo@gmail.com

****************************************************************************/

#ifndef Foundation_Functional_h
#define Foundation_Functional_h


#include <string>
#include <sstream>
#include <vector>
#include <regex>

namespace Foundation{

    namespace Functional{
    /**
    * @brief Converts basic numeric types to strings.
    * @param[in]  val         The basic numeric.
    * @param[in]  percision   Number of precision.
    * @usage tostring(val);
    */
    template<class _Val>
    std::string tostring(_Val val,int precision = 2)
    {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(precision) << val;
        return oss.str();
    }

    /**
    * @brief Converts boolean types to strings.
    * @usage tostring(val);
    */
    inline std::string tostring(bool val)
    {
        return val ? "true" : "false";
    }

    /**
    * @brief Converts strings to basic numeric types.
    * @usage tonumber<type>( string );
    */
    template< class _Type, class _Val >
    _Type tonumber(_Val& val)
    {
        std::istringstream iss(val);
        _Type result;
        return (iss >> result) ? result : 0;
    }


    std::string tolower(const std::string& str);
    std::string toupper(const std::string& str);


    std::string format(const char* format, ...);


    // trim from start
    std::string &ltrim(std::string &s);

    // trim from end
    std::string &rtrim(std::string &s);

    // trim from both ends
    std::string &trim(std::string &s);

    std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);

    std::vector<std::string> split(const std::string &s, char delim);

    template< typename ContainerDest, typename ContainerSource>
    void containerAppend(ContainerDest& dest, ContainerSource& source, size_t index, size_t count)
    {
        typename ContainerSource::iterator begin = source.begin();
        std::advance(begin, index);
        typename ContainerSource::iterator end = begin;
        std::advance(end, count);
        dest.resize(dest.size() + count);
        std::copy_backward(begin, end, dest.end());
    }

    template< typename T >
    void tokenize( const std::string& str, std::vector< T >& tokens, const std::string delimiters )
    {
        std::regex splitPattern(delimiters); 

        std::sregex_token_iterator i(str.begin(), str.end(), splitPattern, -1); 
        std::sregex_token_iterator end; 

        for(; i != end; i++)
        {
            if( (*i).matched)
            {
                T temp; 
                std::stringstream inStream(*i); 
                inStream >> temp; 
                tokens.push_back(temp);
            }
        }
    }


    } //namespace Functional
} //namespace Foundation
#endif // Foundation_Functional_h


