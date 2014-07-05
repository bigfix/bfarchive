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

void ReadFile( const char* path, Stream& stream )
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

}
