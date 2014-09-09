#ifndef Foundation_BitStream_h
#define Foundation_BitStream_h

#include <assert.h>
#include "Buffer.h"
#include "Endian.h"

namespace Foundation {

struct Point3F
{
   float x; ///< the X coordinate
   float y; ///< the Y coordinate
   float z; ///< the Z coordinate
};

inline unsigned int getBinLog2(unsigned int num, bool knownPow2 = false)
{
	// Taken from: http://graphics.stanford.edu/~seander/bithacks.html

	static const unsigned int MultiplyDeBruijnBitPosition[32] = 
	{
		0, 1, 28, 2, 29, 14, 24, 3, 30, 22, 20, 15, 25, 17, 4, 8, 
		31, 27, 13, 23, 21, 19, 16, 7, 26, 12, 18, 6, 11, 5, 10, 9
	};

	if (!knownPow2)
	{
		num |= num >> 1; // first round down to power of 2 
		num |= num >> 2;
		num |= num >> 4;
		num |= num >> 8;
		num |= num >> 16;
		num = (num >> 1) + 1;
	}

	return MultiplyDeBruijnBitPosition[(num * 0x077CB531UL) >> 27];
}
inline bool isPow2(const unsigned int num)
{
	return (num & (num - 1)) == 0;
}
inline unsigned int getNextBinLog2(unsigned int number)
{
	return getBinLog2(number) + (isPow2(number) ? 0 : 1);
}

/** 
/// Helper macro used in BitStream declaration.
///
/// @note DeclareTemplatizedReadWrite macro declares a read and write function
///       for the specified type. This is a macro because MSVC6 has seriously
///       broken template functionality.  Thanks MS!
 */
#define DeclareTemplatizedReadWrite(T) \
inline bool write(T value) { T temp = convertHostToLEndian(value); return write(sizeof(T), &temp); } \
inline bool read(T *value) { T temp; bool success = read(sizeof(T), &temp); *value = convertLEndianToHost(temp); return success;}

/// BitStream provides a bit-level stream interface to a data buffer.
class BitStream : public Foundation::Buffer<unsigned char>
{
protected:
   enum {
	  ResizePad = 1500,
   };
   unsigned int  bitNum;              ///< The current bit position for reading/writing in the bit stream.
   bool error;                        ///< Flag set if a user operation attempts to read or write past the max read/write sizes.
   bool mCompressRelative;            ///< Flag set if the bit stream should compress points relative to a compression point.
   Point3F mCompressPoint;            ///< Reference point for relative point compression.
   unsigned int  maxReadBitNum;       ///< Last valid read bit position.
   unsigned int  maxWriteBitNum;      ///< Last valid write bit position.

   static const size_t DefaultBufferSize = 512;

   bool resizeBits(unsigned int numBitsNeeded);
public:
  
   /// @name Constructors
   ///
   /// Note that the BitStream essentially wraps an existing buffer, so to use a bitstream you must
   /// have an existing buffer for it to work with!
   ///
   /// @{

   /// Default to maximum write size being the size of the buffer.
   BitStream(unsigned char *bufPtr, std::size_t bufSize) : 
	   Buffer(bufPtr, bufSize) 
   { 
	   setMaxSizes(bufSize, bufSize); reset(); 
   }

   /// Optionally, specify a maximum write size.
   BitStream(unsigned char *bufPtr, std::size_t bufSize, unsigned int maxWriteSize): 
	   Buffer(bufPtr, bufSize)
   { 
	   setMaxSizes(bufSize, maxWriteSize); reset();
   }

   /// Creates a resizable BitStream
   BitStream(std::size_t bytes = DefaultBufferSize) :
	   Buffer(bytes)
   { 
	   setMaxSizes( size(), size() ); 
	   reset(); 
   }
   /// @}

   /// Sets the maximum read and write sizes for the BitStream.
   void setMaxSizes(unsigned int maxReadSize, unsigned int maxWriteSize = 0);

   /// Sets the maximum read and write bit sizes for the BitStream.
   void setMaxBitSizes(unsigned int maxReadBitSize, unsigned int maxWriteBitSize = 0);

   /// resets the read/write position to 0 and clears any error state.
   void reset();

   /// sets the ConnectionStringTable for compressing string table entries across the network

   /// clears the error state from an attempted read or write overrun
   void clearError() { error = false; }

   /// Returns a pointer to the next byte in the BitStream from the current bit position
   unsigned char*  getBytePtr();

   /// Returns the current position in the stream rounded up to the next byte.
   unsigned int getBytePosition() const;
   /// Returns the current bit position in the stream
   unsigned int getBitPosition() const;
   /// Sets the position in the stream to the first bit of byte newPosition.
   void setBytePosition(const unsigned int newPosition);
   /// Sets the position in the stream to newBitPosition.
   void setBitPosition(const unsigned int newBitPosition);
   /// Advances the position in the stream by numBits.
   void advanceBitPosition(const int numBits);

   /// Returns the maximum readable bit position
   unsigned int getMaxReadBitPosition() const { return maxReadBitNum; }

   /// Returns the number of bits that can be written into the BitStream without resizing
   unsigned int getBitSpaceAvailable() const { return maxWriteBitNum - bitNum; }

   /// Pads the bits up to the next byte boundary with 0's.
   void zeroToByteBoundary();

   template<typename T>
   void write_T(T v)
   {
	   unsigned char * pbuffer = getBytePtr();
	   ::memcpy(pbuffer,&v,sizeof(T));
	   unsigned int pos = getBytePosition();
	   pos += sizeof(T);
	   setBytePosition( pos );
   };
   
   template<typename T>
   inline T read_T()
   {
	   size_t size = sizeof(T);
	   T ret;
	   unsigned char * pbuffer = getBytePtr();
	   ::memcpy(&ret,pbuffer,size);
	   unsigned int pos = getBytePosition();
	   pos += size;
	   setBytePosition( pos );
	   return ret;
   }

   /// Writes an unsigned integer value between 0 and 2^(bitCount - 1) into the stream.
   void writeInt(unsigned int value);
   /// Reads an unsigned integer value between 0 and 2^(bitCount - 1) from the stream.
   unsigned int  readInt();

   void writeShort(unsigned short value);
   unsigned short readShort();
	
	void writeByte(unsigned char value );
	unsigned char readByte();

   /// Writes an unsigned integer value between 0 and 2^(bitCount -1) into the stream at the specified position, without changing the current write position.
   void writeIntAt(unsigned int value, unsigned char bitCount, unsigned int bitPosition);

   /// Writes an unsigned integer value in the range rangeStart to rangeEnd inclusive.
   void writeRangedU32(unsigned int value, unsigned int rangeStart, unsigned int rangeEnd);
   /// Reads an unsigned integer value in the range rangeStart to rangeEnd inclusive.
   unsigned int  readRangedU32(unsigned int rangeStart, unsigned int rangeEnd);

   /// Writes an enumeration value in the range of 0 ... enumRange - 1.
   void writeEnum(unsigned int enumValue, unsigned int enumRange);
   /// Reads an enumeration value in the range 0 ... enumRange - 1.
   unsigned int readEnum(unsigned int enumRange);

   /// Writes a float from 0 to 1 inclusive, using bitCount bits of precision.
   void writeFloat(float f);
   /// Reads a float from 0 to 1 inclusive, using bitCount bits of precision.
   float  readFloat();

   void writeDouble( double f );
   double readDouble();

   void writeLongLong( long long nInt64);
   long long readLongLong();

   /// Writes an object's class ID, given its class type and class group.
   void writeClassId(unsigned int classId, unsigned int classType, unsigned int classGroup);
   /// Reads a class ID for an object, given a class type and class group.  Returns -1 if the class type is out of range
   unsigned int readClassId(unsigned int classType, unsigned int classGroup);

   /// Uses the same method as in writeNormalVector to reduce the precision of a normal vector
   /// to determine what will be read from the stream.
   static Point3F dumbDownNormal(const Point3F& vec, unsigned char bitCount);

   /// Writes a normalized vector by writing a z value and theta angle.
   void writeNormalVector(const Point3F& vec, unsigned char angleBitCount, unsigned char zBitCount);
   /// Reads a normalized vector by reading a z value and theta angle.
   void readNormalVector(Point3F *vec, unsigned char angleBitCount, unsigned char zBitCount);

   /// Sets a reference point for subsequent compressed point writing.
   void setPointCompression(const Point3F &p);
   /// Disables compression of point.
   void clearPointCompression();
   /// Writes a point into the stream, to a precision denoted by scale.
   void writePointCompressed(const Point3F &p, float scale);
   /// Reads a compressed point from the stream, to a precision denoted by scale.
   void readPointCompressed(Point3F *p, float scale);

   /// Writes bitCount bits into the stream from bitPtr.
   bool writeBits(unsigned int bitCount, const void *bitPtr);
   /// Reads bitCount bits from the stream into bitPtr.
   bool readBits(unsigned int bitCount, void *bitPtr);

   /// Writes a ByteBuffer into the stream.  The ByteBuffer can be no larger than 1024 bytes in size.
   bool write(const Buffer *theBuffer);

   /// Reads a ByteBuffer in from the stream.
   bool read(Buffer *theBuffer);

   /// Writes a single boolean flag (bit) into the stream, and returns the boolean that was written.
   ///
   /// This is set up so you can do...
   ///
   /// @code
   ///   if(stream->writeFlag(foo == bar))
   ///   {
   ///     ... write other stuff ...
   ///   }
   /// @endcode
   bool writeFlag(bool val);

   /// Reads a single bit from the stream.
   ///
   /// This is set up so you can do...
   ///
   /// @code
   ///   if(stream->readFlag())
   ///   {
   ///     ... read other stuff ...
   ///   }
   /// @endcode
   bool readFlag();

   bool write(bool value) { writeFlag(value); return !error; }
   bool read(bool *value) { *value = readFlag(); return !error; }

   /// Writes a huffman compressed string into the stream.
	// 写指定长度的字符串，forceLen代表字符个数，为0表示不固定长度，字符串以0结尾
   void writeString(const char *stringBuf, unsigned int forceLen = 0);
   /// Reads a huffman compressed string from the stream.
   //void readString(char stringBuf[256]);
	
	// 读指定长度的字符串，forceLen代表字符个数，为0表示不固定长度，字符串以0结尾
   const char * readString( unsigned int forceLen = 0 );

	// 写指定长度的字符串，forceLen代表字符个数，为0表示不固定长度，字符串以0结尾
   void writeWString( const wchar_t * wstring, unsigned int forceLen = 0 );
	
	// 读指定长度的字符串，forceLen代表字符个数，为0表示不固定长度，字符串以0结尾
   const wchar_t * readWString(unsigned int forceLen = 0);


   /// Writes byte data into the stream.
   bool write(const unsigned int in_numBytes, const void* in_pBuffer);
   /// Reads byte data from the stream.
   bool read(const unsigned int in_numBytes,  void* out_pBuffer);

   /// @name Various types that the BitStream can read and write...
   /// @{

   ///
   DeclareTemplatizedReadWrite(unsigned char);
   DeclareTemplatizedReadWrite(char);
   DeclareTemplatizedReadWrite(unsigned short);
   DeclareTemplatizedReadWrite(short);
   DeclareTemplatizedReadWrite(unsigned int);
   DeclareTemplatizedReadWrite(int);
   DeclareTemplatizedReadWrite(long long);
   DeclareTemplatizedReadWrite(unsigned long long);
   DeclareTemplatizedReadWrite(float);
   DeclareTemplatizedReadWrite(double);

   /// @}

   /// Sets the bit at position bitCount to the value of set
   bool setBit(unsigned int bitCount, bool set);
   /// Tests the value of the bit at position bitCount.
   bool testBit(unsigned int bitCount);

   /// Returns whether the BitStream writing has exceeded the write target size.
   bool isFull() { return bitNum > (size() << 3); }
   /// Returns whether the stream has generated an error condition due to reading or writing past the end of the buffer.
   bool isValid() { return !error; }

   /// Hashes the BitStream, writing the hash digest into the end of the buffer, and then encrypts with the given cipher
   
   /// Decrypts the BitStream, then checks the hash digest at the end of the buffer to validate the contents
};

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------


inline unsigned int BitStream::getBytePosition() const
{
   return (bitNum + 7) >> 3;
}

inline unsigned int BitStream::getBitPosition() const
{
   return bitNum;
}

inline void BitStream::setBytePosition(const unsigned int newPosition)
{
   bitNum = newPosition << 3;
}

inline void BitStream::setBitPosition(const unsigned int newBitPosition)
{
   bitNum = newBitPosition;
}

inline void BitStream::advanceBitPosition(const int numBits)
{
   setBitPosition(getBitPosition() + numBits);
}

inline bool BitStream::write(const unsigned int in_numBytes, const void* in_pBuffer)
{
   return writeBits(in_numBytes << 3, in_pBuffer);
}

inline bool BitStream::read(const unsigned int in_numBytes,  void* out_pBuffer)
{
   return readBits(in_numBytes << 3, out_pBuffer);
}

inline bool BitStream::readFlag()
{
   if(bitNum > maxReadBitNum)
   {
	  error = true;
	  //assert(false&&"Out of range read");
	  return false;
   }
   int mask = 1 << (bitNum & 0x7);
   bool ret = (*(Buffer::begin() + (bitNum >> 3)) & mask) != 0;
   bitNum++;
   return ret;
}

};

#endif // Foundation_BitStream_h