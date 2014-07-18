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

#include "DataRef.h"
#include "BigFix/Error.h"
#include <string.h>

namespace BigFix
{

uint8_t DataRef::operator[]( size_t n ) const
{
  if ( n >= Length() )
    throw Error( "Bad subscript" );

  return m_start[n];
}

DataRef DataRef::Slice( size_t start, size_t length )
{
  if ( start + length > Length() )
    throw Error( "Bad slice range" );

  return DataRef( m_start + start, m_start + start + length );
}

}

bool operator==( BigFix::DataRef a, BigFix::DataRef b )
{
  return ( a.Length() == b.Length() ) &&
         ( memcmp( a.Start(), b.Start(), a.Length() ) == 0 );
}
