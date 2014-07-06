#include "UnixFilesystem.h"
#include "BigFix/DataRef.h"
#include "BigFix/Error.h"
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <unistd.h>

namespace BigFix
{

UnixFile::UnixFile( int fd, const std::string& path )
  : m_fd( fd ), m_path( path )
{
}

UnixFile::~UnixFile()
{
  close( m_fd );
}

void UnixFile::SetModificationTime( const DateTime& mtime )
{
  struct tm systemTime;
  memset( &systemTime, 0, sizeof( systemTime ) );

  systemTime.tm_year = mtime.Year() - 1900;
  systemTime.tm_mon = mtime.Month() - 1;
  systemTime.tm_mday = mtime.Day();
  systemTime.tm_wday = mtime.DayOfWeek() - 1;
  systemTime.tm_hour = mtime.Hour();
  systemTime.tm_min = mtime.Minute();
  systemTime.tm_sec = mtime.Second();

  time_t unixTime = timegm( &systemTime );

  struct timeval fileTimes[2];
  fileTimes[0].tv_sec = unixTime;
  fileTimes[1].tv_sec = unixTime;

  if ( utimes( m_path.c_str(), fileTimes ) )
    throw Error( "Failed to set modification time" );
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

static std::auto_ptr<File> NewFile( const char* path, int fd )
{
  if ( fd < 0 )
    throw Error( "Failed to open or create file" );

  std::auto_ptr<File> file;

  try
  {
    file.reset( new UnixFile( fd, path ) );
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
    path,
    open( path,
          O_WRONLY | O_CREAT | O_EXCL,
          S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH ) );
}

std::auto_ptr<File> OpenExistingFile( const char* path )
{
  return NewFile( path, open( path, O_RDONLY ) );
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

  struct tm result;
  if ( gmtime_r( &stats.st_mtime, &result ) == 0 )
    throw Error( "Failed to convert file time to DateTime" );

  DateTime mtime( result.tm_year + 1900,
                  result.tm_mon + 1,
                  result.tm_mday,
                  result.tm_wday + 1,
                  result.tm_hour,
                  result.tm_min,
                  result.tm_sec );

  return FileStatus(
    stats.st_size, mtime, S_ISDIR( stats.st_mode ), S_ISREG( stats.st_mode ) );
}

std::string JoinFilePath( const std::string& parent, const std::string& child )
{
  if ( parent.empty() || parent == "." )
    return child;

  return parent + "/" + child;
}

void StreamStdIn( Stream& stream )
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
