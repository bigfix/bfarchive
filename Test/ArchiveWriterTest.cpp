#include "BigFix/ArchiveWriter.cpp"
#include <gtest/gtest.h>

using namespace BigFix;

class VectorStream : public Stream
{
public:
  VectorStream() : ended( false ) {}

  virtual void Write( DataRef data )
  {
    output.insert( output.end(), data.Start(), data.End() );
  }

  virtual void End() { ended = true; }

  std::vector<uint8_t> output;
  bool ended;
};

TEST( ArchiveWriterTest, EmptyArchive )
{
  VectorStream output;
  ArchiveWriter writer( output );

  writer.End();

  uint8_t expected[] = { 0x5f, 0x00 };

  EXPECT_TRUE( output.ended );

  const std::vector<uint8_t>& actual = output.output;
  ASSERT_EQ( sizeof( expected ), actual.size() );

  ASSERT_TRUE( std::equal( actual.begin(), actual.end(), expected ) );
}

TEST( ArchiveWriterTest, BasicArchive )
{
  VectorStream output;
  ArchiveWriter writer( output );

  writer.Directory( "hello",
                    ENCODING_LOCAL,
                    DateTime( DataRef( "Tue, 01 Jul 2014 07:23:00 +0000" ) ) );

  writer.FileStart( "hello/world.txt",
                    ENCODING_LOCAL,
                    DateTime( DataRef( "Tue, 01 Jul 2014 07:23:00 +0000" ) ),
                    13 );
  writer.FileWrite( DataRef( "Hello, world!" ) );
  writer.FileEnd();

  writer.FileStart( "hello/empty.txt",
                    ENCODING_LOCAL,
                    DateTime( DataRef( "Tue, 01 Jul 2014 07:23:00 +0000" ) ),
                    0 );
  writer.FileEnd();

  writer.End();

  uint8_t expected[] =
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

  EXPECT_TRUE( output.ended );

  const std::vector<uint8_t>& actual = output.output;
  ASSERT_EQ( sizeof( expected ), actual.size() );
  
  ASSERT_TRUE( std::equal( actual.begin(), actual.end(), expected ) );
}

TEST( ArchiveWriterTest, HugeFile )
{
  VectorStream output;
  ArchiveWriter writer( output );

  writer.FileStart( "huge_file",
                    ENCODING_LOCAL,
                    DateTime( DataRef( "Tue, 01 Jul 2014 07:54:26 +0000" ) ),
                    4294967296 );

  uint8_t expected[] =
  {
    0x31, 0x0a, 0x68, 0x75, 0x67, 0x65, 0x5f, 0x66, 0x69, 0x6c, 0x65, 0x00,
    0x1f, 0x54, 0x75, 0x65, 0x2c, 0x20, 0x30, 0x31, 0x20, 0x4a, 0x75, 0x6c,
    0x20, 0x32, 0x30, 0x31, 0x34, 0x20, 0x30, 0x37, 0x3a, 0x35, 0x34, 0x3a,
    0x32, 0x36, 0x20, 0x2b, 0x30, 0x30, 0x30, 0x30, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x00, 0x00, 0x00
  };

  const std::vector<uint8_t>& actual = output.output;
  ASSERT_EQ( sizeof( expected ), actual.size() );
  
  ASSERT_TRUE( std::equal( actual.begin(), actual.end(), expected ) );
}

TEST( ArchiveWriterTest, UTF8File )
{
  const uint8_t konnichiwa[] =
  {
    0xe3, 0x81, 0x93, 0xe3, 0x82, 0x93, 0xe3, 0x81, 0xab, 0xe3, 0x81, 0xa1,
    0xe3, 0x81, 0xaf, 0x00
  };

  VectorStream output;
  ArchiveWriter writer( output );

  writer.FileStart( reinterpret_cast<const char*>( konnichiwa ),
                    ENCODING_UTF8,
                    DateTime( DataRef( "Tue, 01 Jul 2014 08:07:02 +0000" ) ),
                    5 );

  writer.FileWrite( DataRef( "hello" ) );
  writer.FileEnd();
  writer.End();

  uint8_t expected[] =
  {
    0x32, 0x5f, 0x10, 0xe3, 0x81, 0x93, 0xe3, 0x82, 0x93, 0xe3, 0x81, 0xab,
    0xe3, 0x81, 0xa1, 0xe3, 0x81, 0xaf, 0x00, 0x1f, 0x54, 0x75, 0x65, 0x2c,
    0x20, 0x30, 0x31, 0x20, 0x4a, 0x75, 0x6c, 0x20, 0x32, 0x30, 0x31, 0x34,
    0x20, 0x30, 0x38, 0x3a, 0x30, 0x37, 0x3a, 0x30, 0x32, 0x20, 0x2b, 0x30,
    0x30, 0x30, 0x30, 0x05, 0x00, 0x00, 0x00, 0x68, 0x65, 0x6c, 0x6c, 0x6f,
    0x5f, 0x00
  };

  EXPECT_TRUE( output.ended );

  const std::vector<uint8_t>& actual = output.output;
  ASSERT_EQ( sizeof( expected ), actual.size() );
  
  ASSERT_TRUE( std::equal( actual.begin(), actual.end(), expected ) );

}
