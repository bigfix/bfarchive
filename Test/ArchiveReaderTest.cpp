#include "BigFix/ArchiveReader.h"
#include "BigFix/ArchiveStream.h"
#include "BigFix/DataRef.h"
#include <gtest/gtest.h>
#include <vector>

using namespace BigFix;

struct ArchiveEntry
{
  bool isDirectory;
  std::string name;
  Encoding nameEncoding;
  DateTime mtime;
  uint64_t length;
  std::string contents;
  bool ended;
};

class TestArchiveStream : public ArchiveStream
{
public:
  TestArchiveStream() : ended( false )
  {
  }

  virtual void Directory( const char* name,
                          Encoding nameEncoding,
                          const DateTime& mtime )
  {
    ArchiveEntry entry = { true, name, nameEncoding, mtime, 0, "", true };
    entries.push_back( entry );
  }

  virtual void FileStart( const char* name,
                          Encoding nameEncoding,
                          const DateTime& mtime,
                          uint64_t length )
  {
    ArchiveEntry entry =
      { false, name, nameEncoding, mtime, length, "", false };
    entries.push_back( entry );
  }

  virtual void FileWrite( DataRef data )
  {
    entries.back().contents.append(
      reinterpret_cast<const char*>( data.Start() ), data.Length() );
  }

  virtual void FileEnd() { entries.back().ended = true; }

  virtual void End() { ended = true; }

  std::vector<ArchiveEntry> entries;
  bool ended;
};

static TestArchiveStream ReadArchive( const uint8_t* start, size_t length )
{
  TestArchiveStream archiveStream;
  ArchiveReader reader( archiveStream );

  for ( const uint8_t* it = start; it != start + length; it ++ )
    reader.Write( DataRef( it, it + 1 ) );
  reader.End();

  return archiveStream;
}

TEST( ArchiveReaderTest, EmptyArchive )
{
  uint8_t data[] = { 0x5f, 0x00 };
  TestArchiveStream output = ReadArchive( data, sizeof( data ) );
  ASSERT_TRUE( output.entries.empty() );
}

TEST( ArchiveReaderTest, BasicArchive )
{
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

  TestArchiveStream output = ReadArchive( data, sizeof( data ) );
  EXPECT_TRUE( output.ended );

  const std::vector<ArchiveEntry>& entries = output.entries;
  ASSERT_EQ( 3, entries.size() );

  EXPECT_TRUE( entries[0].isDirectory );
  EXPECT_EQ( "hello/", entries[0].name );
  EXPECT_EQ( ENCODING_LOCAL, entries[0].nameEncoding );
  EXPECT_EQ( "Tue, 01 Jul 2014 07:23:00 +0000", entries[0].mtime.ToString() );

  EXPECT_FALSE( entries[1].isDirectory );
  EXPECT_EQ( "hello/world.txt", entries[1].name );
  EXPECT_EQ( ENCODING_LOCAL, entries[1].nameEncoding );
  EXPECT_EQ( "Tue, 01 Jul 2014 07:23:00 +0000", entries[1].mtime.ToString() );
  EXPECT_EQ( "Hello, world!", entries[1].contents );
  EXPECT_EQ( 13, entries[1].length );
  EXPECT_TRUE( entries[1].ended );

  EXPECT_FALSE( entries[2].isDirectory );
  EXPECT_EQ( "hello/empty.txt", entries[2].name );
  EXPECT_EQ( ENCODING_LOCAL, entries[2].nameEncoding );
  EXPECT_EQ( "Tue, 01 Jul 2014 07:23:00 +0000", entries[2].mtime.ToString() );
  EXPECT_EQ( "", entries[2].contents );
  EXPECT_EQ( 0, entries[2].length );
  EXPECT_TRUE( entries[2].ended );
}

TEST( ArchiveReaderTest, HugeFile )
{
  uint8_t data[] =
  {
    0x31, 0x0a, 0x68, 0x75, 0x67, 0x65, 0x5f, 0x66, 0x69, 0x6c, 0x65, 0x00,
    0x1f, 0x54, 0x75, 0x65, 0x2c, 0x20, 0x30, 0x31, 0x20, 0x4a, 0x75, 0x6c,
    0x20, 0x32, 0x30, 0x31, 0x34, 0x20, 0x30, 0x37, 0x3a, 0x35, 0x34, 0x3a,
    0x32, 0x36, 0x20, 0x2b, 0x30, 0x30, 0x30, 0x30, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x00, 0x00, 0x00
  };

  TestArchiveStream output = ReadArchive( data, sizeof( data ) );
  EXPECT_FALSE( output.ended );

  const std::vector<ArchiveEntry>& entries = output.entries;
  ASSERT_EQ( 1, entries.size() );

  EXPECT_FALSE( entries[0].isDirectory );
  EXPECT_EQ( "huge_file", entries[0].name );
  EXPECT_EQ( ENCODING_LOCAL, entries[0].nameEncoding );
  EXPECT_EQ( "Tue, 01 Jul 2014 07:54:26 +0000", entries[0].mtime.ToString() );
  EXPECT_EQ( 4294967296, entries[0].length );
  EXPECT_EQ( "", entries[0].contents );
}

TEST( ArchiveReaderTest, UTF8File )
{
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

  TestArchiveStream output = ReadArchive( data, sizeof( data ) );
  EXPECT_TRUE( output.ended );

  const std::vector<ArchiveEntry>& entries = output.entries;
  ASSERT_EQ( 1, entries.size() );

  EXPECT_FALSE( entries[0].isDirectory );
  EXPECT_EQ( reinterpret_cast<const char*>( konnichiwa ), entries[0].name );
  EXPECT_EQ( ENCODING_UTF8, entries[0].nameEncoding );
  EXPECT_EQ( "Tue, 01 Jul 2014 08:07:02 +0000", entries[0].mtime.ToString() );
  EXPECT_EQ( 5, entries[0].length );
  EXPECT_EQ( "hello", entries[0].contents );
  EXPECT_TRUE( entries[0].ended );
}
