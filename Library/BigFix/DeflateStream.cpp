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

#include "DeflateStream.h"
#include "BigFix/DataRef.h"
#include "BigFix/Error.h"
#include <string.h>

namespace BigFix
{

DeflateStream::DeflateStream( Stream& output )
  : m_output( output ), m_sentHeader( false )
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
    WriteHeader();

  if ( data.IsEmpty() )
    return;

  uint8_t buffer[4096];

  m_zstream.next_in = const_cast<uint8_t*>( data.Start() );
  m_zstream.avail_in = static_cast<unsigned int>( data.Length() );

  do
  {
    m_zstream.next_out = buffer;
    m_zstream.avail_out = sizeof( buffer );

    if ( deflate( &m_zstream, Z_NO_FLUSH ) == Z_STREAM_ERROR )
      throw Error( "Failed to compress data" );

    m_output.Write( DataRef( buffer, m_zstream.next_out ) );
  }
  while ( m_zstream.avail_out == 0 );
}

void DeflateStream::End()
{
  if ( !m_sentHeader )
    WriteHeader();

  uint8_t buffer[4096];

  m_zstream.next_in = Z_NULL;
  m_zstream.avail_in = 0;

  do
  {
    m_zstream.next_out = buffer;
    m_zstream.avail_out = sizeof( buffer );

    if ( deflate( &m_zstream, Z_FINISH ) == Z_STREAM_ERROR )
      throw Error( "Failed to compress data" );

    m_output.Write( DataRef( buffer, m_zstream.next_out ) );
  }
  while ( m_zstream.avail_out == 0 );

  m_output.End();
}

void DeflateStream::WriteHeader()
{
  m_output.Write( DataRef( "##SC001\0\0\0\0\0" ) );
  m_sentHeader = true;  
}

}
