#ifndef BigFix_DateTime_h
#define BigFix_DateTime_h

#include <stdint.h>
#include <time.h>

namespace BigFix
{

class DateTime
{
};

time_t ReadDate( const uint8_t* buffer, size_t length );
void WriteDate( time_t timestamp, uint8_t* buffer, size_t length );

}

#endif
