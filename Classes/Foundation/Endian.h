#ifndef Foundation_Endian_h
#define Foundation_Endian_h


namespace Foundation {

#ifndef LITTLE_ENDIAN
#  define LITTLE_ENDIAN
#endif

inline unsigned char endianSwap(const unsigned char in_swap)
{
   return in_swap;
}

inline char endianSwap(const char in_swap)
{
   return in_swap;
}

/**
   Convert the byte ordering on the unsigned short to and from big/little endian format.
   @param in_swap Any unsigned short
   @returns swapped unsigned short.
 */

inline unsigned short endianSwap(const unsigned short in_swap)
{
   return ( unsigned short) (((in_swap >> 8) & 0x00ff) |
              ((in_swap << 8) & 0xff00));
}

inline short endianSwap(const short in_swap)
{
   return (short)(endianSwap((unsigned short)(in_swap)));
}

/**
   Convert the byte ordering on the unsigned int to and from big/little endian format.
   @param in_swap Any unsigned int
   @returns swapped unsigned int.
 */
inline unsigned int endianSwap(const unsigned int in_swap)
{
   return (unsigned int)(((in_swap >> 24) & 0x000000ff) |
              ((in_swap >>  8) & 0x0000ff00) |
              ((in_swap <<  8) & 0x00ff0000) |
              ((in_swap << 24) & 0xff000000));
}

inline int endianSwap(const int in_swap)
{
   return (int)(endianSwap((unsigned int)(in_swap)));
}

inline unsigned long long endianSwap(const unsigned long long in_swap)
{
   unsigned int *inp = (unsigned int *) &in_swap;
   unsigned long long ret;
   unsigned int *outp = (unsigned int *) &ret;
   outp[0] = endianSwap(inp[1]);
   outp[1] = endianSwap(inp[0]);
   return ret;
}

inline long long endianSwap(const long long in_swap)
{
   return (long long)(endianSwap((unsigned long long)(in_swap)));
}

inline float endianSwap(const float in_swap)
{
   unsigned int result = endianSwap(* ((unsigned int *) &in_swap) );
   return * ((float *) &result);
}

inline double endianSwap(const double in_swap)
{
   unsigned long long result = endianSwap(* ((unsigned long long *) &in_swap) );
   return * ((double *) &result);
}

//------------------------------------------------------------------------------
// Endian conversions
#ifdef LITTLE_ENDIAN

#define DECLARE_TEMPLATIZED_ENDIAN_CONV(type) \
   inline type convertHostToLEndian(type i) { return i; } \
   inline type convertLEndianToHost(type i) { return i; } \
   inline type convertHostToBEndian(type i) { return endianSwap(i); } \
   inline type convertBEndianToHost(type i) { return endianSwap(i); }

#elif defined(BIG_ENDIAN)

#define DECLARE_TEMPLATIZED_ENDIAN_CONV(type) \
   inline type convertHostToLEndian(type i) { return endianSwap(i); } \
   inline type convertLEndianToHost(type i) { return endianSwap(i); } \
   inline type convertHostToBEndian(type i) { return i; } \
   inline type convertBEndianToHost(type i) { return i; }

#else
#error "Endian define not set!"
#endif


DECLARE_TEMPLATIZED_ENDIAN_CONV(unsigned char);
DECLARE_TEMPLATIZED_ENDIAN_CONV(char);
DECLARE_TEMPLATIZED_ENDIAN_CONV(unsigned short);
DECLARE_TEMPLATIZED_ENDIAN_CONV(short);
DECLARE_TEMPLATIZED_ENDIAN_CONV(unsigned int);
DECLARE_TEMPLATIZED_ENDIAN_CONV(int);
DECLARE_TEMPLATIZED_ENDIAN_CONV(unsigned long long);
DECLARE_TEMPLATIZED_ENDIAN_CONV(long long);
DECLARE_TEMPLATIZED_ENDIAN_CONV(float);
DECLARE_TEMPLATIZED_ENDIAN_CONV(double);

};

#endif // Foundation_Endian_h