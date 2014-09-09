/****************************************************************************
  Copyright (c) 2013-2014 libo.
 
  losemymind.libo@gmail.com

****************************************************************************/

#ifndef Foundation_Functional_h
#define Foundation_Functional_h


#include <string>
#include <sstream>
#include <vector>

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

    } //namespace Functional
} //namespace Foundation
#endif // Foundation_Functional_h


