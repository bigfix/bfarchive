/*
  Copyright 2014 International Business Machines, Inc.

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
*/

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
    number += data[i - 1];
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
    uint8_t c = data[i];

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
