/****************************************************************************
  Copyright (c) 2014-2015 libo.
 
  losemymind.libo@gmail.com

****************************************************************************/
#ifndef Foundation_FoundationMacros_h
#define Foundation_FoundationMacros_h

// 当编译器支持C++11的 '=default' 和 '=delete' 后，将这两宏定义为1
#define FOUNDATION_NO_DEFAULTED_FUNCTIONS 0   
#define FOUNDATION_NO_DELETED_FUNCTIONS   0

/*
 * only certain compilers support __attribute__((deprecated))
 */
#if defined(__GNUC__) && ((__GNUC__ >= 4) || ((__GNUC__ == 3) && (__GNUC_MINOR__ >= 1)))
    #define DEPRECATED_ATTRIBUTE __attribute__((deprecated))
#elif _MSC_VER >= 1400 //vs 2005 or higher
    #define DEPRECATED_ATTRIBUTE __declspec(deprecated) 
#else
    #define DEPRECATED_ATTRIBUTE
#endif 

#endif // Foundation_FoundationMacros_h