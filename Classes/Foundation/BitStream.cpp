
#include "BitStream.h"
#include <math.h>

namespace Foundation {

    static const float FloatOne  = float(1.0);                           ///< Constant float 1.0
    static const float FloatHalf = float(0.5);                           ///< Constant float 0.5
    static const float FloatZero = float(0.0);                           ///< Constant float 0.0

    static const float FloatPi   = float(3.14159265358979323846);            ///< Constant float PI
    static const float Float2Pi  = float(2.0 * 3.14159265358979323846);      ///< Constant float 2*PI
    static const float FloatInversePi = float(1.0 / 3.14159265358979323846); ///< Constant float 1 / PI
    static const float FloatHalfPi = float(0.5 * 3.14159265358979323846);    ///< Constant float 1/2 * PI
    static const float Float2InversePi = float(2.0 / 3.14159265358979323846);///< Constant float 2 / PI
    static const float FloatInverse2Pi = float(0.5 / 3.14159265358979323846);///< Constant float 2 / PI

    static const float FloatSqrt2 = float(1.41421356237309504880f);          ///< Constant float sqrt(2)
    static const float FloatSqrtHalf = float(0.7071067811865475244008443f);  ///< Constant float sqrt(0.5)

    void BitStream::setMaxSizes(unsigned int maxReadSize, unsigned int maxWriteSize)
    {
        maxReadBitNum = maxReadSize << 3;
        maxWriteBitNum = maxWriteSize << 3;
    }

    void BitStream::setMaxBitSizes(unsigned int maxReadSize, unsigned int maxWriteSize)
    {
        maxReadBitNum = maxReadSize;
        maxWriteBitNum = maxWriteSize;
    }

    void BitStream::reset()
    {
        bitNum = 0;
        error = false;
        mCompressRelative = false;
        clear();
    }

    unsigned char *BitStream::getBytePtr()
    {
        return Buffer::begin() + getBytePosition();
    }

    bool BitStream::resizeBits(unsigned int newBits)
    {
        unsigned int newSize = ((maxWriteBitNum + newBits + 7) >> 3) + ResizePad;
        resize(newSize);
        maxReadBitNum = newSize << 3;
        maxWriteBitNum = newSize << 3;
        return true;
    }

    bool BitStream::writeBits(unsigned int bitCount, const void *bitPtr)
    {
        if(!bitCount)
            return true;

        if(bitCount + bitNum > maxWriteBitNum)
            if(!resizeBits(bitCount + bitNum - maxWriteBitNum))
                return false;

        unsigned int upShift  = bitNum & 0x7;
        unsigned int downShift= 8 - upShift;

        const unsigned char *sourcePtr = (unsigned char *) bitPtr;
        unsigned char *destPtr = Buffer::begin() + (bitNum >> 3);

        // if this write is for <= 1 byte, and it will all fit in the
        // first dest byte, then do some special masking.
        if(downShift >= bitCount)
        {
            unsigned char mask = ((1 << bitCount) - 1) << upShift;
            *destPtr = (*destPtr & ~mask) | ((*sourcePtr << upShift) & mask);
            bitNum += bitCount;
            return true;
        }

        // check for byte aligned writes -- this will be
        // much faster than the shifting writes.
        if(!upShift) 
        {
            bitNum += bitCount;
            for(; bitCount >= 8; bitCount -= 8)
                *destPtr++ = *sourcePtr++;
            if(bitCount)
            {
                unsigned char mask = (1 << bitCount) - 1;
                *destPtr = (*sourcePtr & mask) | (*destPtr & ~mask);
            }
            return true;
        }

        // the write destination is not byte aligned.
        unsigned char sourceByte;
        unsigned char destByte = *destPtr & (0xFF >> downShift);
        unsigned char lastMask  = 0xFF >> (7 - ((bitNum + bitCount - 1) & 0x7));

        bitNum += bitCount;

        for(;bitCount >= 8; bitCount -= 8)
        {
            sourceByte = *sourcePtr++;
            *destPtr++ = destByte | (sourceByte << upShift);
            destByte = sourceByte >> downShift;
        }
        if(bitCount == 0)
        {
            *destPtr = (*destPtr & ~lastMask) | (destByte & lastMask);
            return true;
        }
        if(bitCount <= downShift)
        {
            *destPtr = (*destPtr & ~lastMask) | ((destByte | (*sourcePtr << upShift)) & lastMask);
            return true;
        }
        sourceByte = *sourcePtr;

        *destPtr++ = destByte | (sourceByte << upShift);
        *destPtr = (*destPtr & ~lastMask) | ((sourceByte >> downShift) & lastMask);
        return true;
    }

    bool BitStream::readBits(unsigned int bitCount, void *bitPtr)
    {
        if(!bitCount)
            return true;
        if(bitCount + bitNum > maxReadBitNum)
        {
            error = true;
            return false;
        }

        unsigned char *sourcePtr = Buffer::begin() + (bitNum >> 3);
        unsigned int byteCount = (bitCount + 7) >> 3;

        unsigned char *destPtr = (unsigned char *) bitPtr;

        unsigned int downShift = bitNum & 0x7;
        unsigned int upShift = 8 - downShift;

        if(!downShift)
        {
            while(byteCount--)
                *destPtr++ = *sourcePtr++;
            bitNum += bitCount;
            return true;
        }

        unsigned char sourceByte = *sourcePtr >> downShift;
        bitNum += bitCount;

        for(; bitCount >= 8; bitCount -= 8)
        {
            unsigned char nextByte = *++sourcePtr;
            *destPtr++ = sourceByte | (nextByte << upShift);
            sourceByte = nextByte >> downShift;
        }
        if(bitCount)
        {
            if(bitCount <= upShift)
            {
                *destPtr = sourceByte;
                return true;
            }
            *destPtr = sourceByte | ( (*++sourcePtr) << upShift);
        }
        return true;
    }

    bool BitStream::setBit(unsigned int bitCount, bool set)
    {
        if(bitCount >= maxWriteBitNum)
            if(!resizeBits(bitCount - maxWriteBitNum + 1))
                return false;
        if(set)
            *(Buffer::begin() + (bitCount >> 3)) |= (1 << (bitCount & 0x7));
        else
            *(Buffer::begin() + (bitCount >> 3)) &= ~(1 << (bitCount & 0x7));
        return true;
    }

    bool BitStream::testBit(unsigned int bitCount)
    {
        return (*(Buffer::begin() + (bitCount >> 3)) & (1 << (bitCount & 0x7))) != 0;
    }

    bool BitStream::writeFlag(bool val)
    {
        if(bitNum + 1 > maxWriteBitNum)
            if(!resizeBits(1))
                return false;
        if(val)
            *(Buffer::begin() + (bitNum >> 3)) |= (1 << (bitNum & 0x7));
        else
            *(Buffer::begin() + (bitNum >> 3)) &= ~(1 << (bitNum & 0x7));
        bitNum++;
        return (val);
    }

  

    unsigned int BitStream::readInt()
    {
        //unsigned int * pFloat = (unsigned int*) getBytePtr();
        //unsigned int ret = *pFloat;

        //unsigned char * pbuffer = getBytePtr();
        //unsigned int ret= 0;
        //
        //::memcpy(&ret,pbuffer,sizeof(unsigned int))= *pFloat;

        //unsigned int pos = getBytePosition();
        //pos += sizeof(unsigned int);
        //setBytePosition( pos );
        //return ret;
        return read_T<unsigned int>();
    }

    void BitStream::writeShort(unsigned short value)
    {
        write_T(value);
    }
    unsigned short BitStream::readShort()
    {
        //unsigned short * pFloat = (unsigned short*) getBytePtr();
        //unsigned short ret = *pFloat;
        //unsigned int pos = getBytePosition();
        //pos += sizeof(unsigned short);
        //setBytePosition( pos );
        //return ret;
        return read_T<unsigned short>();
    }

    void BitStream::writeInt(unsigned int val)
    {

        write_T(val);
        //unsigned int * pInt = (unsigned int*) getBytePtr();
        //*pInt = val;
        //unsigned int pos = getBytePosition();
        //pos += sizeof(unsigned int);
        //setBytePosition( pos );
    }

    void BitStream::writeByte(unsigned char value )
    {
        //unsigned char * pByte = getBytePtr();
        //*pByte = value;
        //unsigned int pos = getBytePosition();
        //pos += sizeof(unsigned char);
        //setBytePosition( pos );

        write_T(value);
    }
    
    unsigned char BitStream::readByte()
    {
        //unsigned char * pByte = getBytePtr();
        //unsigned char ret = *pByte;
        //unsigned int pos = getBytePosition();
        //pos += sizeof(unsigned char);
        //setBytePosition( pos );
        //return ret;
        return read_T<unsigned char>();
    }

    void BitStream::writeFloat(float f)
    {
        //float * pFloat = (float*) getBytePtr();
        ////*pFloat = f;
        //memcpy( pFloat, &f, sizeof(float));
        //unsigned int pos = getBytePosition();
        //pos += sizeof(float);
        //setBytePosition( pos );

        write_T(f);
    }

    float BitStream::readFloat()
    {
        //float * pFloat = (float*) getBytePtr();
        //float ret;
        //memcpy(&ret, pFloat, sizeof(float));
        ////= (*pFloat);
        //unsigned int pos = getBytePosition();
        //pos += sizeof(float);
        //setBytePosition( pos );
        //return ret;

        return read_T<float>();
    }

    void BitStream::writeDouble( double f )
    {
        //double * pFloat = (double*) getBytePtr();
        //*pFloat = f;
        //unsigned int pos = getBytePosition();
        //pos += sizeof(double);
        //setBytePosition( pos );
        write_T(f);
    }
    double BitStream::readDouble()
    {
        //double * pFloat = (double*) getBytePtr();
        //double ret = *pFloat;
        //unsigned int pos = getBytePosition();
        //pos += sizeof(double);
        //setBytePosition( pos );
        //return ret;
        return read_T<double>();
    }

    void BitStream::writeLongLong( long long nInt64)
    {
        //long long * pLongLong = (long long*) getBytePtr();
        //*pLongLong = nInt64;
        //unsigned int pos = getBytePosition();
        //pos += sizeof(long long);
        //setBytePosition( pos );
        write_T(nInt64);
    }

    long long BitStream::readLongLong()
    {
        //long long * pLongLong = (long long*) getBytePtr();
        //long long ret = *pLongLong;
        //unsigned int pos = getBytePosition();
        //pos += sizeof(long long);
        //setBytePosition( pos );
        //return ret;
        return read_T<long long>();
    }




    //----------------------------------------------------------------------------

    void BitStream::clearPointCompression()
    {
        mCompressRelative = false;
    }

    void BitStream::setPointCompression(const Point3F& p)
    {
        mCompressRelative = true;
        mCompressPoint = p;
    }

    static unsigned int gBitCounts[4] = {
        16, 18, 20, 32
    };



    const char * BitStream::readString(unsigned int forceLen)
    {
        const char * sztemp = (const char *) getBytePtr();
        unsigned int pos = getBytePosition();
        if ( forceLen > 0 )
            pos += forceLen;
        else
            pos += ( strlen( sztemp ) + 1 );
        setBytePosition( pos );
        
        return sztemp;
    }

    void BitStream::writeString(const char *string, unsigned int forceLen)
    {
        int sLen = strlen(string) + 1;
        sLen = forceLen - sLen;
        strcpy ( (char *)getBytePtr() , string );
        unsigned int pos = getBytePosition();
        pos += ( strlen( string ) + 1 );
        if ( sLen > 0 )
            pos += sLen;
        setBytePosition( pos );
    }

    void BitStream::writeWString( const wchar_t * wstring, unsigned int forceLen)
    {
        int count = 1;
        const wchar_t * pShort = wstring;
        while (pShort && *pShort)
        {
            ++count;
            writeShort((unsigned short)*pShort);
            ++pShort;
        }
        writeShort(0);
        count = forceLen - count;
        if ( count > 0 )
        {
            unsigned int pos = getBytePosition();
            pos += count * sizeof(unsigned short);
            setBytePosition( pos );
        }
    }

    const wchar_t * BitStream::readWString(unsigned int forceLen)
    {
        const int  bufflen = 2048;
        static wchar_t sztemp[bufflen];
        memset(sztemp, 0, bufflen);
        unsigned int i = 0;
        
        if (forceLen > 0)
        {
            for ( ;  i < forceLen; ++i)
            {
                sztemp[i] = readShort();
            }
        }
        
        else
        {
            for ( ;  i < bufflen; ++i)
            {
                sztemp[i] = readShort();
                if ( sztemp[i] == 0 )
                    break;
            }
        }

        return sztemp;
    }
};
