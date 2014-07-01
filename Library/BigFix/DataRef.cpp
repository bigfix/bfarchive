#include "DataRef.h"
#include "BigFix/Error.h"

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
