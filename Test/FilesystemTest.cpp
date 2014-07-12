#include "BigFix/Error.h"
#include "BigFix/DataRef.h"
#include "BigFix/Filesystem.h"
#include "TestUtility.h"
#include <algorithm>
#include <gtest/gtest.h>

using namespace BigFix;

static std::string Sandbox( const std::string& path )
{
  return JoinPath( "FilesystemTestSandbox", path );
}

TEST( FilesystemTest, IsDots )
{
  EXPECT_TRUE( IsDots( "." ) );
  EXPECT_TRUE( IsDots( ".." ) );
  EXPECT_FALSE( IsDots( "..." ) );
  EXPECT_FALSE( IsDots( "x.." ) );
  EXPECT_FALSE( IsDots( "..x" ) );
  EXPECT_FALSE( IsDots( "" ) );
}

TEST( FilesystemTest, IsAscii )
{
  EXPECT_TRUE( IsAscii( "" ) );
  EXPECT_TRUE( IsAscii( "hello" ) );
  EXPECT_FALSE( IsAscii( "\x80" ) );
}

TEST( FilesystemTest, JoinPath )
{
  EXPECT_EQ( "bar", JoinPath( "", "bar" ) );
  EXPECT_EQ( "bar", JoinPath( ".", "bar" ) );
  EXPECT_EQ( "foo/bar", JoinPath( "foo", "bar" ) );
}

TEST( FilesystemTest, MakeDir )
{
  std::string name = Sandbox( "MakeDirTest" );
  MakeDir( name.c_str() );
  EXPECT_THROW( MakeDir( name.c_str() ), Error );
}

TEST( FilesystemTest, OpenExistingFailsIfDoesntExist )
{
  EXPECT_THROW(
    OpenExistingFile( Sandbox( "OpenExistingFailsIfDoesntExist" ).c_str() ),
    Error );
}

TEST( FilesystemTest, OpenNewFailsIfExists )
{
  std::string name = Sandbox( "OpenNewFailsIfExists" );
  OpenNewFile( name.c_str() );
  EXPECT_THROW( OpenNewFile( name.c_str() ), Error );
}

TEST( FilesystemTest, FileReadAndWrite )
{
  std::string name = Sandbox( "FileReadAndWriteTest" );
  DataRef hello( "hello" );

  {
    std::auto_ptr<File> file = OpenNewFile( name.c_str() );
    file->Write( hello );
  }

  {
    std::auto_ptr<File> file = OpenExistingFile( name.c_str() );

    uint8_t buffer[32];
    size_t nread = file->Read( buffer, sizeof( buffer ) );

    EXPECT_TRUE( hello == DataRef( buffer, buffer + nread ) );
  }
}

TEST( FilesystemTest, SetAndGetModificationTime )
{
  std::string fileName = Sandbox( "SetAndGetModificationTime" );
  DateTime mtime( DataRef( "Sun, 11 Mar 1984 08:23:42 +0000" ) );

  OpenNewFile( fileName.c_str() )->SetModificationTime( mtime );

  FileStatus status = Stat( fileName.c_str() );
  EXPECT_EQ( mtime.ToString(), status.ModificationTime().ToString() );
}

TEST( FilesystemTest, Stat )
{
  MakeDir( Sandbox( "StatDir" ).c_str() );
  OpenNewFile( Sandbox( "StatFile" ).c_str() )->Write( DataRef( "hello" ) );

  FileStatus dirStatus = Stat( Sandbox( "StatDir" ).c_str() );
  EXPECT_TRUE( dirStatus.IsDirectory() );
  EXPECT_FALSE( dirStatus.IsFile() );

  FileStatus fileStatus = Stat( Sandbox( "StatFile" ).c_str() );
  EXPECT_EQ( 5ul, fileStatus.Length() );
  EXPECT_FALSE( fileStatus.IsDirectory() );
  EXPECT_TRUE( fileStatus.IsFile() );
}

TEST( FilesystemTest, ReadDir )
{
  std::string dirName = Sandbox( "ReadDir" );
  MakeDir( dirName.c_str() );

  OpenNewFile( JoinPath( dirName, "a" ).c_str() );
  MakeDir( JoinPath( dirName, "b" ).c_str() );
  OpenNewFile( JoinPath( dirName, "c" ).c_str() );

  std::vector<std::string> expected;
  expected.push_back( "a" );
  expected.push_back( "b" );
  expected.push_back( "c" );

  std::vector<std::string> actual = ReadDir( dirName.c_str() );
  std::sort( actual.begin(), actual.end() );

  EXPECT_EQ( expected, actual );
}

TEST( FilesystemTest, FileStream )
{
  std::string fileName = Sandbox( "FileStream" );
  DataRef hello( "hello" );

  FileStream fileStream;
  fileStream.Reset( OpenNewFile( fileName.c_str() ) );

  fileStream.Write( hello );
  fileStream.End();

  {
    std::auto_ptr<File> file = OpenExistingFile( fileName.c_str() );

    uint8_t buffer[32];
    size_t nread = file->Read( buffer, sizeof( buffer ) );

    EXPECT_TRUE( hello == DataRef( buffer, buffer + nread ) );
  }
}

TEST( FilesystemTest, StreamFile )
{
  std::string fileName = Sandbox( "StreamFile" );
  DataRef hello( "hello" );

  OpenNewFile( fileName.c_str() )->Write( hello );

  std::string output;
  StringStream stringStream( output );
  StreamFile( fileName.c_str(), stringStream );

  EXPECT_EQ( "hello", output );
  EXPECT_TRUE( stringStream.ended );
}

TEST( FilesystemTest, ThrowsIfStatNonexistentFile )
{
  std::string fileName = Sandbox( "MissingFile" );
  EXPECT_THROW( Stat( fileName.c_str() ), Error );
}

TEST( FilesystemTest, ThrowsIfReadNonexistentDirectory )
{
  std::string fileName = Sandbox( "MissingDirectory" );
  EXPECT_THROW( ReadDir( fileName.c_str() ), Error );
}
