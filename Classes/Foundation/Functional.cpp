/****************************************************************************
  Copyright (c) 2013-2014 libo.
 
  losemymind.libo@gmail.com

****************************************************************************/

#include <algorithm>
#include <functional>
#include <locale>
#include <stdarg.h>
#include <cctype>

#include "Functional.h"

namespace Foundation{
    namespace Functional{
    std::string tolower( const std::string& str )
    {
        std::string ret;
        for (auto ch : str)
        {
            if (ch >= 'A' && ch <= 'Z')
                ret += ch | 32;
            else
                ret += ch;
        }
        return ret;
    }

    std::string toupper( const std::string& str )
    {
        std::string ret;
        for (auto ch : str)
        {
            if (ch >= 'a' && ch <= 'z')
                ret += ch | 32;
            else
                ret += ch;
        }
        return ret;
    }

    std::string format( const char* format, ... )
    {
        char buf[1024]= {0};
        va_list arglist;
        va_start(arglist, format);
        vsnprintf(buf, 1024, format, arglist);
        va_end(arglist);
        return buf;
    }

    std::string & ltrim( std::string &s )
    {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
        return s;
    }

    std::string & rtrim( std::string &s )
    {
        s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
        return s;
    }

    std::string & trim( std::string &s )
    {
        return ltrim(rtrim(s));
    }

    std::vector<std::string> & split( const std::string &s, char delim, std::vector<std::string> &elems )
    {
        std::stringstream ss(s);
        std::string item;
        while (std::getline(ss, item, delim))
        {
            elems.push_back(item);
        }
        return elems;
    }

    std::vector<std::string> split( const std::string &s, char delim )
    {
        std::vector<std::string> elems;
        split(s, delim, elems);
        return elems;
    }

    } // namespace Functional
} //namespace Framework