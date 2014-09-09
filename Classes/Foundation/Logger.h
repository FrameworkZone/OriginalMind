/****************************************************************************
  Copyright (c) 2014-2015 libo.
 
  losemymind.libo@gmail.com

****************************************************************************/

#ifndef Foundation_Logger_h
#define Foundation_Logger_h

namespace Foundation{

class Logger
{
public:
    /** 
     * WARNING: if you change the levels don't forget to update
     * levelStrings[] in Logger.cpp
     */
    enum Level { NONE, ERROR, WARN, INFO, DEBUG, FINE, BAD_LEVEL=255 };
};



}
#endif // Foundation_Logger_h