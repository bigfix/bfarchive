#ifndef BigFix_Number_h
#define BigFix_Number_h

#include <stddef.h>
#include <stdint.h>

namespace BigFix
{

class DataRef;

uint64_t ReadLittleEndian( DataRef );
void WriteLittleEndian( uint64_t, uint8_t* buffer, size_t length );

template <class T>
T ReadAsciiNumber( DataRef );

}

#endif
