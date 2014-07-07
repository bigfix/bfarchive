#include "WindowsFilesystem.h"
#include "BigFix/Error.h"
#include "BigFix/DataRef.h"
#include <fcntl.h>
#include <io.h>

namespace BigFix
{

WindowsFile::WindowsFile( HANDLE handle )
  : m_handle( handle )
{
}

WindowsFile::~WindowsFile()
{
  CloseHandle( m_handle );
}

void WindowsFile::SetModificationTime( const DateTime& mtime )
{
  SYSTEMTIME systemTime;
  memset( &systemTime, 0, sizeof( systemTime ) );

  systemTime.wYear = mtime.Year();
  systemTime.wMonth = mtime.Month();
  systemTime.wDayOfWeek = mtime.DayOfWeek() - 1;
  systemTime.wDay = mtime.Day();
  systemTime.wHour = mtime.Hour();
  systemTime.wMinute = mtime.Minute();
  systemTime.wSecond = mtime.Second();

  FILETIME fileTime;
  if ( !SystemTimeToFileTime( &systemTime, &fileTime ) )
    throw Error( "Failed to convert modification time to file time" );

  if ( !SetFileTime( m_handle, NULL, NULL, &fileTime ) )
    throw Error( "Failed to set modification time" );
}

size_t WindowsFile::Read( uint8_t* buffer, size_t length )
{
  DWORD nread;

  if ( !ReadFile(
         m_handle, buffer, static_cast<DWORD>( length ), &nread, NULL ) )
  {
    throw Error( "Failed to read file" );
  }

  return nread;
}

void WindowsFile::Write( DataRef data )
{
  const uint8_t* start = data.Start();
  const uint8_t* end = data.End();

  while ( start != end )
  {
    DWORD nwritten;

    if ( !WriteFile( m_handle,
                     start,
                     static_cast<DWORD>( end - start ),
                     &nwritten,
                     NULL ) )
    {
      throw Error( "Failed to write file" );
    }

    start += nwritten;
  }
}

static std::wstring MakeWindowsFilePath( const char* path )
{
  wchar_t output[1024];

  if ( MultiByteToWideChar( CP_UTF8,
                            MB_ERR_INVALID_CHARS,
                            path,
                            -1,
                            output,
                            _countof( output ) ) == 0 )
  {
    throw Error( "Failed to transcode utf-8 to utf-16" );
  }

  for ( wchar_t* it = output; *it; it++ )
    if ( *it == '/' )
      *it = '\\';

  return output;
}

std::string MakePortableFilePath( const wchar_t* path )
{
  char output[1024];

  if ( WideCharToMultiByte( CP_UTF8,
                            WC_ERR_INVALID_CHARS,
                            path,
                            -1,
                            output,
                            sizeof( output ),
                            NULL,
                            NULL ) == 0 )
  {
    throw Error( "Failed to transcode utf-16 to utf-8" );
  }

  for ( char* it = output; *it; it++ )
    if ( *it == '\\' )
      *it = '/';

  return output; 
}

static std::auto_ptr<File> NewFile( const char* path,
                                    DWORD desiredAccess,
                                    DWORD creationDisposition )
{
  std::wstring windowsPath = MakeWindowsFilePath( path );

  HANDLE handle = CreateFile( windowsPath.c_str(),
                              desiredAccess,
                              0,
                              NULL,
                              creationDisposition,
                              FILE_ATTRIBUTE_NORMAL,
                              NULL );

  if ( handle == INVALID_HANDLE_VALUE )
    throw Error( "Failed to open or create file" );

  std::auto_ptr<File> file;

  try
  {
    file.reset( new WindowsFile( handle ) );
  }
  catch ( ... )
  {
    CloseHandle( handle );
    throw;
  }

  return file;
}

std::auto_ptr<File> OpenNewFile( const char* path )
{
  return NewFile( path, GENERIC_WRITE, CREATE_NEW );
}

std::auto_ptr<File> OpenExistingFile( const char* path )
{
  return NewFile( path, GENERIC_READ, OPEN_EXISTING );
}

void MakeDir( const char* path )
{
  std::wstring windowsPath = MakeWindowsFilePath( path );

  if ( !CreateDirectory( windowsPath.c_str(), NULL ) )
    throw Error( "Failed to create directory" );
}

FindHandle::FindHandle( HANDLE handle ) : m_handle( handle )
{
}

FindHandle::~FindHandle()
{
  if ( m_handle != INVALID_HANDLE_VALUE )
    FindClose( m_handle );
}

FileStatus Stat( const char* path )
{
  std::wstring windowsPath = MakeWindowsFilePath( path );

  WIN32_FIND_DATA findData;
  FindHandle findHandle( FindFirstFile( windowsPath.c_str(), &findData ) );

  if ( findHandle == INVALID_HANDLE_VALUE )
    throw Error( "Failed to stat file" );

  LARGE_INTEGER fileSize;
  fileSize.HighPart = findData.nFileSizeHigh;
  fileSize.LowPart = findData.nFileSizeLow;

  SYSTEMTIME systemTime;
  if ( !FileTimeToSystemTime( &findData.ftLastWriteTime, &systemTime ) )
    throw Error( "Failed to convert file time to system time" );

  DateTime mtime( systemTime.wYear,
                  static_cast<uint8_t>( systemTime.wMonth ),
                  static_cast<uint8_t>( systemTime.wDay ),
                  static_cast<uint8_t>( systemTime.wDayOfWeek + 1 ),
                  static_cast<uint8_t>( systemTime.wHour ),
                  static_cast<uint8_t>( systemTime.wMinute ),
                  static_cast<uint8_t>( systemTime.wSecond ) );

  bool isDirectory =
    ( findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) ? true : false;

  return FileStatus( fileSize.QuadPart, mtime, isDirectory, !isDirectory );
}

void StreamStdIn( Stream& stream )
{
  _setmode( 0, O_BINARY );

  uint8_t buffer[4096];

  while ( true )
  {
    int nread = _read( 0, buffer, sizeof( buffer ) );

    if ( nread < 0 )
      throw Error( "Failed to read from stdin" );

    if ( nread == 0 )
      break;

    stream.Write( DataRef( buffer, buffer + nread ) );
  }

  stream.End();
}

std::vector<std::string> ReadDir( const char* path )
{
  std::wstring windowsPath = MakeWindowsFilePath( path );
  windowsPath += L"\\*";

  std::vector<std::string> entries;

  WIN32_FIND_DATA findData;
  FindHandle findHandle( FindFirstFile( windowsPath.c_str(), &findData ) );

  if ( findHandle == INVALID_HANDLE_VALUE )
  {
    if ( GetLastError() == ERROR_FILE_NOT_FOUND )
      return entries;

    throw Error( "Failed to read directory" );
  }

  while ( true )
  {
    std::string child = MakePortableFilePath( findData.cFileName );

    if ( !IsDots( child.c_str() ) )
      entries.push_back( child );

    if ( !FindNextFile( findHandle, &findData ) )
    {
      if ( GetLastError() == ERROR_NO_MORE_FILES )
        break;

      throw Error( "Failed to read directory" );
    }
  }

  return entries;
}

std::string LocalPathToUTF8Path( const char* path )
{
  return LocalPathToUTF8Path( path, CP_ACP );
}

std::string LocalPathToUTF8Path( const char* path, int codepage )
{
  if ( IsAscii( path ) )
    return path;

  wchar_t utf16[1024];

  if ( MultiByteToWideChar( codepage,
                            MB_ERR_INVALID_CHARS,
                            path,
                            -1,
                            utf16,
                            _countof( utf16 ) ) == 0 )
  {
    throw Error( "Failed to transcode local path to utf-16" );
  }

  char utf8[1024];

  if ( WideCharToMultiByte( CP_UTF8,
                            WC_ERR_INVALID_CHARS,
                            utf16,
                            -1,
                            utf8,
                            sizeof( utf8 ),
                            NULL,
                            NULL ) == 0 )
  {
    throw Error( "Failed to transcode utf-16 to utf-8" );
  }

  return utf8;
}

}
