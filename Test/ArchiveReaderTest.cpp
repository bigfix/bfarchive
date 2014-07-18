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

#include "BigFix/ArchiveReader.h"
#include "BigFix/ArchiveStream.h"
#include "BigFix/DataRef.h"
#include "BigFix/Error.h"
#include "TestUtility.h"
#include <gtest/gtest.h>

using namespace BigFix;

struct ArchiveEntry
{
  ArchiveEntry( bool isDirectory,
                const char* name,
                const DateTime& mtime,
                uint64_t length )
    : isDirectory( isDirectory )
    , name( name )
    , mtime( mtime )
    , length( length )
    , ended( false )
  {
  }

  bool isDirectory;
  std::string name;
  DateTime mtime;
  uint64_t length;
  std::string contents;
  bool ended;
};

class TestFileStream : public Stream
{
public:
  void Reset( ArchiveEntry& entry ) { m_entry = &entry; }

  virtual void Write( DataRef data )
  {
    m_entry->contents.insert(
      m_entry->contents.end(), data.Start(), data.End() );
  }

  virtual void End()
  {
    m_entry->ended = true;
    m_entry = 0;
  }

private:
  ArchiveEntry* m_entry;
};

class TestArchiveStream : public ArchiveStream
{
public:
  TestArchiveStream() : ended( false )
  {
  }

  virtual void Directory( const char* name, const DateTime& mtime )
  {
    entries.push_back( ArchiveEntry( true, name, mtime, 0 ) );
  }

  virtual Stream& File( const char* name,
                        const DateTime& mtime,
                        uint64_t length )
  {
    entries.push_back( ArchiveEntry( false, name, mtime, length ) );
    testFileStream.Reset( entries.back() );
    return testFileStream;
  }

  virtual void End() { ended = true; }

  std::vector<ArchiveEntry> entries;
  bool ended;
  TestFileStream testFileStream;
};

TEST( ArchiveReaderTest, EmptyArchive )
{
  TestArchiveStream output;
  ArchiveReader reader( output );

  uint8_t data[] = { 0x5f, 0x00 };

  WriteOneByOneAndEnd( reader, DataRef( data, data + sizeof( data ) ) );

  EXPECT_TRUE( output.entries.empty() );
  EXPECT_TRUE( output.ended );
}

TEST( ArchiveReaderTest, BasicArchive )
{
  TestArchiveStream output;
  ArchiveReader reader( output );

  uint8_t data[] =
  {
    0x5f, 0x07, 0x68, 0x65, 0x6c, 0x6c, 0x6f, 0x2f, 0x00, 0x1f, 0x54, 0x75,
    0x65, 0x2c, 0x20, 0x30, 0x31, 0x20, 0x4a, 0x75, 0x6c, 0x20, 0x32, 0x30,
    0x31, 0x34, 0x20, 0x30, 0x37, 0x3a, 0x32, 0x33, 0x3a, 0x30, 0x30, 0x20,
    0x2b, 0x30, 0x30, 0x30, 0x30, 0x00, 0x00, 0x00, 0x00, 0x5f, 0x10, 0x68,
    0x65, 0x6c, 0x6c, 0x6f, 0x2f, 0x77, 0x6f, 0x72, 0x6c, 0x64, 0x2e, 0x74,
    0x78, 0x74, 0x00, 0x1f, 0x54, 0x75, 0x65, 0x2c, 0x20, 0x30, 0x31, 0x20,
    0x4a, 0x75, 0x6c, 0x20, 0x32, 0x30, 0x31, 0x34, 0x20, 0x30, 0x37, 0x3a,
    0x32, 0x33, 0x3a, 0x30, 0x30, 0x20, 0x2b, 0x30, 0x30, 0x30, 0x30, 0x0d,
    0x00, 0x00, 0x00, 0x48, 0x65, 0x6c, 0x6c, 0x6f, 0x2c, 0x20, 0x77, 0x6f,
    0x72, 0x6c, 0x64, 0x21, 0x5f, 0x10, 0x68, 0x65, 0x6c, 0x6c, 0x6f, 0x2f,
    0x65, 0x6d, 0x70, 0x74, 0x79, 0x2e, 0x74, 0x78, 0x74, 0x00, 0x1f, 0x54,
    0x75, 0x65, 0x2c, 0x20, 0x30, 0x31, 0x20, 0x4a, 0x75, 0x6c, 0x20, 0x32,
    0x30, 0x31, 0x34, 0x20, 0x30, 0x37, 0x3a, 0x32, 0x33, 0x3a, 0x30, 0x30,
    0x20, 0x2b, 0x30, 0x30, 0x30, 0x30, 0x00, 0x00, 0x00, 0x00, 0x5f, 0x00
  };

  WriteOneByOneAndEnd( reader, DataRef( data, data + sizeof( data ) ) );

  const std::vector<ArchiveEntry>& entries = output.entries;
  ASSERT_EQ( 3ul, entries.size() );

  EXPECT_TRUE( entries[0].isDirectory );
  EXPECT_EQ( "hello/", entries[0].name );
  EXPECT_EQ( "Tue, 01 Jul 2014 07:23:00 +0000", entries[0].mtime.ToString() );

  EXPECT_FALSE( entries[1].isDirectory );
  EXPECT_EQ( "hello/world.txt", entries[1].name );
  EXPECT_EQ( "Tue, 01 Jul 2014 07:23:00 +0000", entries[1].mtime.ToString() );
  EXPECT_EQ( "Hello, world!", entries[1].contents );
  EXPECT_EQ( 13ull, entries[1].length );
  EXPECT_TRUE( entries[1].ended );

  EXPECT_FALSE( entries[2].isDirectory );
  EXPECT_EQ( "hello/empty.txt", entries[2].name );
  EXPECT_EQ( "Tue, 01 Jul 2014 07:23:00 +0000", entries[2].mtime.ToString() );
  EXPECT_EQ( "", entries[2].contents );
  EXPECT_EQ( 0ull, entries[2].length );
  EXPECT_TRUE( entries[2].ended );

  EXPECT_TRUE( output.ended );
}

TEST( ArchiveReaderTest, HugeFile )
{
  TestArchiveStream output;
  ArchiveReader reader( output );

  uint8_t data[] =
  {
    0x31, 0x0a, 0x68, 0x75, 0x67, 0x65, 0x5f, 0x66, 0x69, 0x6c, 0x65, 0x00,
    0x1f, 0x54, 0x75, 0x65, 0x2c, 0x20, 0x30, 0x31, 0x20, 0x4a, 0x75, 0x6c,
    0x20, 0x32, 0x30, 0x31, 0x34, 0x20, 0x30, 0x37, 0x3a, 0x35, 0x34, 0x3a,
    0x32, 0x36, 0x20, 0x2b, 0x30, 0x30, 0x30, 0x30, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x00, 0x00, 0x00
  };

  WriteOneByOneAndEnd( reader, DataRef( data, data + sizeof( data ) ) );

  const std::vector<ArchiveEntry>& entries = output.entries;
  ASSERT_EQ( 1ul, entries.size() );

  EXPECT_FALSE( entries[0].isDirectory );
  EXPECT_EQ( "huge_file", entries[0].name );
  EXPECT_EQ( "Tue, 01 Jul 2014 07:54:26 +0000", entries[0].mtime.ToString() );
  EXPECT_EQ( 4294967296ull, entries[0].length );
  EXPECT_EQ( "", entries[0].contents );

  EXPECT_FALSE( output.ended );
}

TEST( ArchiveReaderTest, UTF8File )
{
  TestArchiveStream output;
  ArchiveReader reader( output );

  const uint8_t konnichiwa[] =
  {
    0xe3, 0x81, 0x93, 0xe3, 0x82, 0x93, 0xe3, 0x81, 0xab, 0xe3, 0x81, 0xa1,
    0xe3, 0x81, 0xaf, 0x00
  };

  uint8_t data[] =
  {
    0x32, 0x5f, 0x10, 0xe3, 0x81, 0x93, 0xe3, 0x82, 0x93, 0xe3, 0x81, 0xab,
    0xe3, 0x81, 0xa1, 0xe3, 0x81, 0xaf, 0x00, 0x1f, 0x54, 0x75, 0x65, 0x2c,
    0x20, 0x30, 0x31, 0x20, 0x4a, 0x75, 0x6c, 0x20, 0x32, 0x30, 0x31, 0x34,
    0x20, 0x30, 0x38, 0x3a, 0x30, 0x37, 0x3a, 0x30, 0x32, 0x20, 0x2b, 0x30,
    0x30, 0x30, 0x30, 0x05, 0x00, 0x00, 0x00, 0x68, 0x65, 0x6c, 0x6c, 0x6f,
    0x5f, 0x00
  };

  WriteOneByOneAndEnd( reader, DataRef( data, data + sizeof( data ) ) );

  const std::vector<ArchiveEntry>& entries = output.entries;
  ASSERT_EQ( 1ul, entries.size() );

  EXPECT_FALSE( entries[0].isDirectory );
  EXPECT_EQ( reinterpret_cast<const char*>( konnichiwa ), entries[0].name );
  EXPECT_EQ( "Tue, 01 Jul 2014 08:07:02 +0000", entries[0].mtime.ToString() );
  EXPECT_EQ( 5ull, entries[0].length );
  EXPECT_EQ( "hello", entries[0].contents );
  EXPECT_TRUE( entries[0].ended );

  EXPECT_TRUE( output.ended );
}

TEST( ArchiveReaderTest, ConvertsBackSlashToForwardSlash )
{
  TestArchiveStream output;
  ArchiveReader reader( output );

  uint8_t data[] =
  {
    0x5f, 0x0d, 0x68, 0x65, 0x6c, 0x6c, 0x6f, 0x5c, 0x77, 0x6f, 0x72, 0x6c,
    0x64, 0x5c, 0x00, 0x1f, 0x54, 0x75, 0x65, 0x2c, 0x20, 0x30, 0x31, 0x20,
    0x4a, 0x75, 0x6c, 0x20, 0x32, 0x30, 0x31, 0x34, 0x20, 0x30, 0x37, 0x3a,
    0x32, 0x33, 0x3a, 0x30, 0x30, 0x20, 0x2b, 0x30, 0x30, 0x30, 0x30, 0x00,
    0x00, 0x00, 0x00, 0x5f, 0x00
  };

  WriteOneByOneAndEnd( reader, DataRef( data, data + sizeof( data ) ) );

  const std::vector<ArchiveEntry>& entries = output.entries;
  ASSERT_EQ( 1ul, entries.size() );
  EXPECT_EQ( "hello/world/", entries[0].name );
}

TEST( ArchiveReaderTest, NullTerminatesPath )
{
  TestArchiveStream output;
  ArchiveReader reader( output );

  uint8_t data[] =
  {
    0x5f, 0x0c, 0x68, 0x65, 0x6c, 0x6c, 0x6f, 0x5c, 0x77, 0x6f, 0x72, 0x6c,
    0x64, 0x5c, 0x1f, 0x54, 0x75, 0x65, 0x2c, 0x20, 0x30, 0x31, 0x20, 0x4a,
    0x75, 0x6c, 0x20, 0x32, 0x30, 0x31, 0x34, 0x20, 0x30, 0x37, 0x3a, 0x32,
    0x33, 0x3a, 0x30, 0x30, 0x20, 0x2b, 0x30, 0x30, 0x30, 0x30, 0x00, 0x00,
    0x00, 0x00, 0x5f, 0x00
  };

  WriteOneByOneAndEnd( reader, DataRef( data, data + sizeof( data ) ) );

  const std::vector<ArchiveEntry>& entries = output.entries;
  ASSERT_EQ( 1ul, entries.size() );
  EXPECT_EQ( "hello/world/", entries[0].name );
}

static void ExpectThrow( DataRef input )
{
  TestArchiveStream output;
  ArchiveReader reader( output );
  EXPECT_THROW( WriteOneByOneAndEnd( reader, input ), Error );
}

TEST( ArchiveReaderTest, DotDotIsError )
{
  // The name is just '..'
  uint8_t test1[] = { 0x5f, 0x03, 0x2e, 0x2e, 0x00 };
  ExpectThrow( DataRef( test1, test1 + sizeof( test1 ) ) );

  // The name is '../'
  uint8_t test2[] = { 0x5f, 0x04, 0x2e, 0x2e, 0x2f, 0x00 };
  ExpectThrow( DataRef( test2, test2 + sizeof( test2 ) ) );

  // The name is 'foo/../bar'
  uint8_t test3[] = { 0x5f, 0x0b, 0x66, 0x6f, 0x6f, 0x2f, 0x2e,
                      0x2e, 0x2f, 0x62, 0x61, 0x72, 0x00 };

  ExpectThrow( DataRef( test3, test3 + sizeof( test3 ) ) );
}

TEST( ArchiveReaderTest, ThrowsIfMoreDataAfterArchiveEnded )
{
  uint8_t data[] = { 0x5f, 0x00, 'h', 'o', 'd', 'o', 'r' };
  ExpectThrow( DataRef( data, data + sizeof( data ) ) );
}

TEST( ArchiveReaderTest, ThrowsOnInvalidHeader )
{
  uint8_t data[] = { 'h', 'o', 'd', 'o', 'r' };
  ExpectThrow( DataRef( data, data + sizeof( data ) ) );
}

TEST( ArchiveReaderTest, ThrowsOnDirectoriesWithNonzeroLength )
{
  uint8_t data[] =
  {
    0x5f, 0x07, 0x68, 0x65, 0x6c, 0x6c, 0x6f, 0x2f, 0x00, 0x1f, 0x54, 0x75,
    0x65, 0x2c, 0x20, 0x30, 0x31, 0x20, 0x4a, 0x75, 0x6c, 0x20, 0x32, 0x30,
    0x31, 0x34, 0x20, 0x30, 0x37, 0x3a, 0x32, 0x33, 0x3a, 0x30, 0x30, 0x20,
    0x2b, 0x30, 0x30, 0x30, 0x30, 0x01, 0x00, 0x00, 0x00, 0x00, 0x5f, 0x00,
  };

  ExpectThrow( DataRef( data, data + sizeof( data ) ) );
}
