#include "UnixFilesystem.h"
#include "BigFix/DataRef.h"
#include "BigFix/Error.h"
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

namespace BigFix
{

UnixFile::UnixFile( int fd ) : m_fd( fd )
{
}

UnixFile::~UnixFile()
{
  close( m_fd );
}

size_t UnixFile::Read( uint8_t* buffer, size_t length )
{
  ssize_t nread = read( m_fd, buffer, length );

  if ( nread < 0 )
    throw Error( "Failed to read file" );

  return nread;
}

void UnixFile::Write( DataRef data )
{
  const uint8_t* start = data.Start();
  const uint8_t* end = data.End();

  while ( start != end )
  {
    ssize_t nwritten = write( m_fd, start, end - start );

    if ( nwritten < 0 )
      throw Error( "Failed to write file" );

    start += nwritten;
  }
}

static std::auto_ptr<File> MakeFile( int fd )
{
  if ( fd < 0 )
    throw Error( "Failed to open or create file" );

  std::auto_ptr<File> file;

  try
  {
    file.reset( new UnixFile( fd ) );
  }
  catch ( ... )
  {
    close( fd );
    throw;
  }

  return file;
}

std::auto_ptr<File> OpenNewFile( const char* name )
{
  return MakeFile( open( name, O_WRONLY | O_CREAT | O_EXCL, S_IRWXU ) );
}

std::auto_ptr<File> OpenExistingFile( const char* name )
{
  return MakeFile( open( name, O_RDONLY ) );
}

void MakeDir( const char* name )
{
  if ( mkdir( name, S_IRWXU ) )
    throw Error( "Failed to create directory" );
}

}
