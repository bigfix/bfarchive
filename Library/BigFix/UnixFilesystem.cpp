#include "UnixFilesystem.h"
#include "BigFix/DataRef.h"
#include "BigFix/Error.h"
#include <fcntl.h>
#include <string.h>
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

static std::auto_ptr<File> NewFile( int fd )
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

std::auto_ptr<File> OpenNewFile( const char* path )
{
  return NewFile(
    open( path,
          O_WRONLY | O_CREAT | O_EXCL,
          S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH ) );
}

std::auto_ptr<File> OpenExistingFile( const char* path )
{
  return NewFile( open( path, O_RDONLY ) );
}

void MakeDir( const char* path )
{
  if ( mkdir( path, S_IRWXU ) )
    throw Error( "Failed to create directory" );
}

FileStatus Stat( const char* path )
{
  struct stat stats;

  if ( stat( path, &stats ) )
    throw Error( "Failed to stat file" );

  return FileStatus(
    stats.st_size, S_ISDIR( stats.st_mode ), S_ISREG( stats.st_mode ) );
}

std::string JoinFilePath( const std::string& parent, const std::string& child )
{
  if ( parent.empty() || parent == "." )
    return child;

  return parent + "/" + child;
}

void ReadStdIn( Stream& stream )
{
  uint8_t buffer[4096];

  while ( true )
  {
    ssize_t nread = read( 0, buffer, sizeof( buffer ) );

    if ( nread < 0 )
      throw Error( "Failed to read from stdin" );

    if ( nread == 0 )
      break;

    stream.Write( DataRef( buffer, buffer + nread ) );
  }

  stream.End();
}

OpenDir::OpenDir( const char* path )
{
  m_dir = opendir( path );

  if ( !m_dir )
    throw Error( "Failed to open directory" );
}

OpenDir::~OpenDir()
{
  closedir( m_dir );
}

static bool IsDots( const char* path )
{
  if ( strcmp( path, "." ) == 0 )
    return true;

  if ( strcmp( path, ".." ) == 0 )
    return true;

  return false;
}

std::vector<std::string> ReadDir( const char* path )
{
  OpenDir dir( path );

  std::vector<std::string> entries;

  while ( true )
  {
    struct dirent entry;
    struct dirent* result;

    if ( readdir_r( dir, &entry, &result ) )
      throw Error( "Failed to read directory contents" );

    if ( !result )
      break;

    if ( !IsDots( result->d_name ) )
      entries.push_back( result->d_name );
  }

  return entries;
}

}
