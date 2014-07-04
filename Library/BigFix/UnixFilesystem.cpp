#include "UnixFilesystem.h"
#include "BigFix/DataRef.h"
#include "BigFix/Error.h"
#include <fcntl.h>
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

std::auto_ptr<File> OpenNewFile( const char* name, Encoding nameEncoding )
{
  int fd = open( name, O_WRONLY | O_CREAT | O_EXCL, S_IRWXU );
  if ( fd < 0 )
    throw Error( "Failed to create file" );

  return MakeFile( fd );
}

}
