#include "DeflateStream.h"
#include "BigFix/DataRef.h"
#include "BigFix/Error.h"
#include <string.h>

namespace BigFix
{

DeflateStream::DeflateStream( Stream& output )
  : m_output( output ), m_sentHeader( false ), m_zstatus( Z_OK )
{
  memset( &m_zstream, 0, sizeof( m_zstream ) );

  if ( deflateInit( &m_zstream, Z_DEFAULT_COMPRESSION ) )
    throw Error( "Failed to initialize zlib" );
}

DeflateStream::~DeflateStream()
{
  deflateEnd( &m_zstream );
}

void DeflateStream::Write( DataRef data )
{
  if ( !m_sentHeader )
  {
    m_output.Write( DataRef( "##SC001\0\0\0\0\0" ) );
    m_sentHeader = true;
  }

  m_zstream.next_in = const_cast<uint8_t*>( data.Start() );
  m_zstream.avail_in = data.Length();
  Pump( Z_NO_FLUSH );
}

void DeflateStream::End()
{
  m_zstream.next_in = Z_NULL;
  m_zstream.avail_in = 0;
  Pump( Z_FINISH );
}

void DeflateStream::Pump( int zflags )
{
  uint8_t buffer[4096];

  while ( m_zstream.avail_in != 0 )
  {
    m_zstream.next_out = buffer;
    m_zstream.avail_out = sizeof( buffer );

    m_zstatus = deflate( &m_zstream, zflags );
    if ( m_zstatus != Z_OK )
      throw Error( "Failed to decompress data" );

    if ( buffer != m_zstream.next_out )
      m_output.Write( DataRef( buffer, m_zstream.next_out ) );
  }
}

}
