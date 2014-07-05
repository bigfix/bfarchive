#include "BigFix/DataRef.h"
#include "BigFix/InflateStream.h"
#include "TestUtility.h"
#include <gtest/gtest.h>

using namespace BigFix;

TEST( InflateStreamTest, ShortRaw )
{
  std::string output;
  StringStream stringStream( output );
  InflateStream inflateStream( stringStream );

  DataRef data( "hello" );

  WriteOneByOneAndEnd( inflateStream, data );

  EXPECT_EQ( "hello", output );
  EXPECT_TRUE( stringStream.ended );
}

TEST( InflateStreamTest, LongRaw )
{
  std::string output;
  StringStream stringStream( output );
  InflateStream inflateStream( stringStream );

  DataRef data( "hello, world! blah blah blah" );

  WriteOneByOneAndEnd( inflateStream, data );

  EXPECT_EQ( "hello, world! blah blah blah", output );
  EXPECT_TRUE( stringStream.ended );
}

TEST( InflateStreamTest, DecompressHelloWorld )
{
  std::string output;
  StringStream stringStream( output );
  InflateStream inflateStream( stringStream );

  uint8_t data[] =
  {
    0x23, 0x23, 0x53, 0x43, 0x30, 0x30, 0x31, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x78, 0x9c, 0xf3, 0x48, 0xcd, 0xc9, 0xc9, 0xd7, 0x51, 0x28, 0xcf, 0x2f,
    0xca, 0x49, 0x51, 0x04, 0x00, 0x20, 0x5e, 0x04, 0x8a
  };

  WriteOneByOneAndEnd( inflateStream, DataRef( data, data + sizeof( data ) ) );

  EXPECT_EQ( "Hello, world!", output );
  EXPECT_TRUE( stringStream.ended );
}

TEST( InflateStreamTest, DecompressEmpty )
{
  std::string output;
  StringStream stringStream( output );
  InflateStream inflateStream( stringStream );

  uint8_t data[] =
  {
    0x23, 0x23, 0x53, 0x43, 0x30, 0x30, 0x31, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x78, 0x9c, 0x03, 0x00, 0x00, 0x00, 0x00, 0x01
  };

  WriteOneByOneAndEnd( inflateStream, DataRef( data, data + sizeof( data ) ) );

  EXPECT_EQ( "", output );
  EXPECT_TRUE( stringStream.ended );
}
