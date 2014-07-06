#include "Filesystem.h"
#include "BigFix/DataRef.h"

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

}
