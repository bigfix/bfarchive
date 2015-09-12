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

#include "Filesystem.h"
#include "BigFix/DataRef.h"
#include <string.h>

namespace BigFix
{

File::~File()
{
}

void FileStream::Reset( std::auto_ptr<File> file )
{
  m_file = file;
}

void FileStream::Write( DataRef data )
{
  m_file->Write( data );
}

void FileStream::End()
{
  m_file.reset();
}

void StreamFile( const char* path, Stream& stream )
{
  std::auto_ptr<File> file = OpenExistingFile( path );

  uint8_t buffer[4096];

  while ( true )
  {
    size_t nread = file->Read( buffer, sizeof( buffer ) );

    if ( nread == 0 )
      break;

    stream.Write( DataRef( buffer, buffer + nread ) );
  }

  stream.End();
}

FileStatus::FileStatus( uint64_t length,
                        const DateTime& mtime,
                        bool isDirectory,
                        bool isFile )
  : m_length( length )
  , m_mtime( mtime )
  , m_isDirectory( isDirectory )
  , m_isFile( isFile )
{
}

bool IsAscii( const char* path )
{
  for ( const char* it = path; *it; it++ )
    if ( static_cast<uint8_t>( *it ) >= 128 )
      return false;

  return true;
}

bool IsDots( const char* path )
{
  if ( strcmp( path, "." ) == 0 )
    return true;

  if ( strcmp( path, ".." ) == 0 )
    return true;

  return false;
}

std::string JoinPath( const std::string& parent, const std::string& child )
{
  if ( parent.empty() || parent == "." )
    return child;

  return parent + "/" + child;
}

std::string PathBasename( const std::string& path )
{
  size_t pos = path.rfind( '/' );

  if ( pos == std::string::npos )
    return path;

  return path.substr( pos + 1 );
}

}
