/****************************************************************************
  Copyright (c) 2013-2014 libo.
 
  losemymind.libo@gmail.com

****************************************************************************/

#ifndef Foundation_noncopyable_h
#define Foundation_noncopyable_h

#include "FoundationMacros.h"

namespace Foundation {
class noncopyable
{
protected:
#ifndef FOUNDATION_NO_DEFAULTED_FUNCTIONS
    noncopyable() = default;
    ~noncopyable() = default;
#else
    noncopyable() {}
    ~noncopyable() {}
#endif
#ifndef FOUNDATION_NO_DELETED_FUNCTIONS
    noncopyable( const noncopyable& ) = delete;
    noncopyable& operator=( const noncopyable& ) = delete;
#else
private:  // emphasize the following members are private
    noncopyable( const noncopyable& );
    noncopyable& operator=( const noncopyable& );
#endif
};
}//namespace Foundation

#endif // Foundation_noncopyable_h
