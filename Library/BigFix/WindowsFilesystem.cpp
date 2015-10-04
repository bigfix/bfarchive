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

#include "WindowsFilesystem.h"
#include "BigFix/Error.h"
#include "BigFix/DataRef.h"
#include <fcntl.h>
#include <io.h>

static std::string ErrorString( int errnum )
{
  wchar_t buffer16[1024];

  DWORD length16 =
    FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                   NULL,
                   errnum,
                   0,
                   buffer16,
                   _countof( buffer16 ),
                   NULL );

  if ( length16 == 0 )
    return "Unknown error";

  char buffer8[1024];

  if ( WideCharToMultiByte( CP_UTF8,
                            0,
                            buffer16,
                            length16,
                            buffer8,
                            sizeof( buffer8 ),
                            NULL,
                            NULL ) == 0 )
  {
    return "Unknown error";
  }

  return buffer8;
}

std::string ErrorString( const std::string& what, int errnum )
{
  return what + ": " + ErrorString( errnum );
}

static std::string FileErrorString( const std::string& what,
                                    const std::string& path,
                                    int errnum )
{
  return ErrorString( what + " " + path, errnum );
}

namespace BigFix
{

WindowsFile::WindowsFile( HANDLE handle, const std::string& path )
  : m_handle( handle ), m_path( path )
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
    throw Error( ErrorString(
      "Failed to convert modification time to file time", GetLastError() ) );

  if ( !SetFileTime( m_handle, NULL, NULL, &fileTime ) )
    throw Error( FileErrorString(
      "Failed to set modification time on", m_path, GetLastError() ) );
}

size_t WindowsFile::Read( uint8_t* buffer, size_t length )
{
  DWORD nread;

  if ( !ReadFile(
         m_handle, buffer, static_cast<DWORD>( length ), &nread, NULL ) )
  {
    throw Error(
      FileErrorString( "Failed to read file", m_path, GetLastError() ) );
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
      throw Error(
        FileErrorString( "Failed to write file", m_path, GetLastError() ) );
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
    throw Error(
      ErrorString( "Failed to transcode utf-8 to utf-16", GetLastError() ) );
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
                            0,
                            path,
                            -1,
                            output,
                            sizeof( output ),
                            NULL,
                            NULL ) == 0 )
  {
    throw Error(
      ErrorString( "Failed to transcode utf-16 to utf-8", GetLastError() ) );
  }

  for ( char* it = output; *it; it++ )
    if ( *it == '\\' )
      *it = '/';

  return output; 
}

static std::auto_ptr<File> OpenFile( const char* path,
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
    throw Error( FileErrorString( "Failed to open", path, GetLastError() ) );

  std::auto_ptr<File> file;

  try
  {
    file.reset( new WindowsFile( handle, path ) );
  }
  catch ( ... )
  {
    CloseHandle( handle );
    throw;
  }

  return file;
}

std::auto_ptr<File> OpenAsNewFile( const char* path )
{
  return OpenFile( path, GENERIC_READ | GENERIC_WRITE, CREATE_ALWAYS );
}

std::auto_ptr<File> OpenExistingFile( const char* path )
{
  return OpenFile( path, GENERIC_READ, OPEN_EXISTING );
}

void MakeDir( const char* path )
{
  std::wstring windowsPath = MakeWindowsFilePath( path );

  if ( CreateDirectory( windowsPath.c_str(), NULL ) )
    return;

  DWORD lastError = GetLastError();

  if ( lastError == ERROR_ALREADY_EXISTS && Stat( path ).IsDirectory() )
    return;

  throw Error(
    FileErrorString( "Failed to create directory", path, lastError ) );
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

  WIN32_FILE_ATTRIBUTE_DATA fileData;
  if ( !GetFileAttributesEx(
         windowsPath.c_str(), GetFileExInfoStandard, &fileData ) )
  {
    throw Error(
      FileErrorString( "Failed to stat file", path, GetLastError() ) );
  }

  LARGE_INTEGER fileSize;
  fileSize.HighPart = fileData.nFileSizeHigh;
  fileSize.LowPart = fileData.nFileSizeLow;

  SYSTEMTIME systemTime;
  if ( !FileTimeToSystemTime( &fileData.ftLastWriteTime, &systemTime ) )
    throw Error( ErrorString( "Failed to convert file time to system time",
                              GetLastError() ) );

  DateTime mtime( systemTime.wYear,
                  static_cast<uint8_t>( systemTime.wMonth ),
                  static_cast<uint8_t>( systemTime.wDay ),
                  static_cast<uint8_t>( systemTime.wDayOfWeek + 1 ),
                  static_cast<uint8_t>( systemTime.wHour ),
                  static_cast<uint8_t>( systemTime.wMinute ),
                  static_cast<uint8_t>( systemTime.wSecond ) );

  bool isDirectory =
    ( fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) ? true : false;

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
      throw Error( ErrorString( "Failed to read from stdin", GetLastError() ) );

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
    DWORD lastError = GetLastError();

    if ( lastError == ERROR_FILE_NOT_FOUND )
      return entries;

    throw Error(
      FileErrorString( "Failed to read directory", path, lastError ) );
  }

  while ( true )
  {
    std::string child = MakePortableFilePath( findData.cFileName );

    if ( !IsDots( child.c_str() ) )
      entries.push_back( child );

    if ( !FindNextFile( findHandle, &findData ) )
    {
      DWORD lastError = GetLastError();

      if ( lastError == ERROR_NO_MORE_FILES )
        break;

      throw Error(
        FileErrorString( "Failed to read directory", path, lastError ) );
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
    throw Error( FileErrorString(
      "Failed to transcode local path to utf-16", path, GetLastError() ) );
  }

  char utf8[1024];

  if ( WideCharToMultiByte( CP_UTF8,
                            0,
                            utf16,
                            -1,
                            utf8,
                            sizeof( utf8 ),
                            NULL,
                            NULL ) == 0 )
  {
    throw Error( FileErrorString(
      "Failed to transcode utf-16 to utf-8", path, GetLastError() ) );
  }

  return utf8;
}

}
