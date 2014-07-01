#include "Number.h"
#include "BigFix/DataRef.h"
#include "BigFix/Error.h"

namespace BigFix
{

uint64_t ReadLittleEndian( DataRef data )
{
  uint64_t number = 0;

  for ( size_t i = data.Length(); i != 0; i-- )
  {
    number *= 256;
    number += data.Start()[i - 1];
  }

  return number;
}

void WriteLittleEndian( uint64_t number, uint8_t* buffer, size_t length )
{
  for ( size_t i = 0; i != length; i++ )
  {
    buffer[i] = number % 256;
    number /= 256;
  }
}

static bool IsAsciiDigit( uint8_t c )
{
  return '0' <= c && c <= '9';
}

template < class T >
T ReadAsciiNumber( DataRef data )
{
  T number = 0;

  for ( size_t i = 0; i < data.Length(); i++ )
  {
    uint8_t c = data.Start()[i];

    if ( !IsAsciiDigit( c ) )
      throw Error( "Invalid ascii number" );

    number *= 10;
    number += c - '0';
  }

  return number;
}

template uint8_t ReadAsciiNumber<uint8_t>( DataRef );
template int32_t ReadAsciiNumber<int32_t>( DataRef );
template uint32_t ReadAsciiNumber<uint32_t>( DataRef );
}
