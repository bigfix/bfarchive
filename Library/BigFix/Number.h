#ifndef BigFix_Number_h
#define BigFix_Number_h

#include <stddef.h>
#include <stdint.h>

namespace BigFix
{

uint64_t ReadNumber( const uint8_t* buffer, size_t length );
void WriteNumber( uint64_t, uint8_t* buffer, size_t length );

}

#endif
