#include "Number.h"

namespace BigFix
{

uint64_t ReadNumber( const uint8_t* buffer, size_t length )
{
  uint64_t number = 0;

  for ( size_t i = length; i != 0; i-- )
  {
    number *= 256;
    number += buffer[i - 1];
  }

  return number;
}

void WriteNumber( uint64_t number, uint8_t* buffer, size_t length )
{
  for ( size_t i = 0; i != length; i++ )
  {
    buffer[i] = number % 256;
    number /= 256;
  }
}

}
