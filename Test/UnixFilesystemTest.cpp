/*
  Copyright 2015 International Business Machines, Inc.

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

#include "BigFix/Error.h"
#include "BigFix/DataRef.h"
#include "BigFix/Filesystem.h"
#include "BigFix/UnixTestSeams.h"
#include "ScopedMock.h"
#include "TestUtility.h"
#include <gtest/gtest.h>
#include <errno.h>

using namespace BigFix;

static int utimesError( const char*, const struct timeval* )
{
  errno = EACCES;
  return -1;
}

TEST( FilesystemTest, SetModificationTimeFails )
{
  std::string fileName = Sandbox( "SetModificationTimeError" );
  DateTime mtime( DataRef( "Sun, 11 Mar 1984 08:23:42 +0000" ) );

  ScopedMock<Type_utimes> guard( utimesError, Real_utimes, Set_utimes );

  try
  {
    OpenAsNewFile( fileName.c_str() )->SetModificationTime( mtime );
    FAIL();
  }
  catch ( const std::exception& err )
  {
    std::string message = err.what();
    EXPECT_TRUE( message.find( "Failed to set modification time" ) !=
                 message.npos );
  }
  catch ( ... ) { FAIL(); }
}

static int readError( int, void*, size_t )
{
  errno = EBADF;
  return -1;
}

TEST( FilesystemTest, ReadFileFails )
{
  std::string fileName = Sandbox( "ReadFileFails" );

  ScopedMock<Type_read> guard( readError, Real_read, Set_read );

  try
  {
    std::auto_ptr<File> file = OpenAsNewFile( fileName.c_str() );

    uint8_t buffer[32];
    size_t nread = file->Read( buffer, sizeof( buffer ) );

    FAIL();
  }
  catch ( const std::exception& err )
  {
    std::string message = err.what();
    EXPECT_TRUE( message.find( "Failed to read file" ) != message.npos );
  }
  catch ( ... ) { FAIL(); }
}

TEST( FilesystemTest, ReadStdInFails )
{
  ScopedMock<Type_read> guard( readError, Real_read, Set_read );

  NullStream ignore;

  try
  {
    StreamStdIn( ignore );
    FAIL();
  }
  catch ( const std::exception& err )
  {
    std::string message = err.what();
    EXPECT_TRUE( message.find( "Failed to read from stdin" ) != message.npos );
  }
  catch ( ... ) { FAIL(); }
}

static int writeError( int, const void*, size_t )
{
  errno = EBADF;
  return -1;
}

TEST( FilesystemTest, WriteFileFails )
{
  std::string name = Sandbox( "FileWriteFails" );
  DataRef hello( "hello" );

  ScopedMock<Type_write> guard( writeError, Real_write, Set_write );

  try
  {
    std::auto_ptr<File> file = OpenAsNewFile( name.c_str() );
    file->Write( hello );

    FAIL();
  }
  catch ( const std::exception& err )
  {
    std::string message = err.what();
    EXPECT_TRUE( message.find( "Failed to write file" ) != message.npos );
  }
  catch ( ... ) { FAIL(); }
}

static int readdir_rFails( DIR*, struct dirent*, struct dirent** )
{
  return EBADF;
}

TEST( FilesystemTest, ReadDirFails )
{
  std::string dirName = Sandbox( "ReadDirFails" );
  MakeDir( dirName.c_str() );

  ScopedMock<Type_readdir_r> guard(
    readdir_rFails, Real_readdir_r, Set_readdir_r );

  try
  {
    ReadDir( dirName.c_str() );
    FAIL();
  }
  catch ( const std::exception& err )
  {
    std::string message = err.what();
    EXPECT_TRUE( message.find( "Failed to read directory" ) != message.npos );
  }
  catch ( ... ) { FAIL(); }
}

static struct tm* gmtime_rFails( const time_t*, struct tm* )
{
  errno = EINVAL;
  return NULL;
}

TEST( FilesystemTest, StatFails )
{
  std::string fileName = Sandbox( "StatFileFails" );

  OpenAsNewFile( fileName.c_str() );

  ScopedMock<Type_gmtime_r> guard( gmtime_rFails, Real_gmtime_r, Set_gmtime_r );

  try
  {
    Stat( fileName.c_str() );
    FAIL();
  }
  catch ( const std::exception& err )
  {
    std::string message = err.what();
    EXPECT_TRUE( message.find( "Failed to convert file time to DateTime" ) !=
                 message.npos );
  }
  catch ( ... ) { FAIL(); }
}
