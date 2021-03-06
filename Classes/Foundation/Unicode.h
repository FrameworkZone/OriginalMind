/****************************************************************************
// by Marius Bancila
// http://www.codeguru.com/cpp/misc/misc/multi-lingualsupport/article.php/c10451/
  Copyright (c)  Marius Bancila.
  Copyright (c) 2014 libo.
  losemymind.libo@gmail.com

****************************************************************************/

#ifndef Foundation_Unicode_h
#define Foundation_Unicode_h

#include <string>

namespace Foundation{

/** 
 * Convert std::wstring to std::string, the string code page is UTF-8.
 */
std::string wstring2string(const std::wstring &input);

/** 
 * Convert std::string to std::wstring, the string code page is UTF-8.
 */
std::wstring string2wstring(const std::string &input);

} // namespace Foundation
#endif // Foundation_Unicode_h
