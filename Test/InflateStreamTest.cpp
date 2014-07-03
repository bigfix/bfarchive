#include "BigFix/DataRef.h"
#include "BigFix/InflateStream.h"
#include "TestUtility.h"
#include <gtest/gtest.h>

using namespace BigFix;

TEST( InflateStreamTest, ShortRaw )
{
  StringStream stringStream;
  InflateStream inflateStream( stringStream );

  DataRef data( "hello" );

  WriteOneByOne( inflateStream, data );
  inflateStream.End();

  EXPECT_EQ( "hello", stringStream.output );
  EXPECT_TRUE( stringStream.ended );
}

TEST( InflateStreamTest, LongRaw )
{
  StringStream stringStream;
  InflateStream inflateStream( stringStream );

  DataRef data( "hello, world! blah blah blah" );

  WriteOneByOne( inflateStream, data );
  inflateStream.End();

  EXPECT_EQ( "hello, world! blah blah blah", stringStream.output );
  EXPECT_TRUE( stringStream.ended );
}

TEST( InflateStreamTest, DecompressHelloWorld )
{
  StringStream stringStream;
  InflateStream inflateStream( stringStream );

  uint8_t data[] =
  {
    0x23, 0x23, 0x53, 0x43, 0x30, 0x30, 0x31, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x78, 0x9c, 0xf3, 0x48, 0xcd, 0xc9, 0xc9, 0xd7, 0x51, 0x28, 0xcf, 0x2f,
    0xca, 0x49, 0x51, 0x04, 0x00, 0x20, 0x5e, 0x04, 0x8a
  };

  WriteOneByOne( inflateStream, DataRef( data, data + sizeof( data ) ) );
  inflateStream.End();

  EXPECT_EQ( "Hello, world!", stringStream.output );
  EXPECT_TRUE( stringStream.ended );
}

TEST( InflateStreamTest, DecompressEmpty )
{
  StringStream stringStream;
  InflateStream inflateStream( stringStream );

  uint8_t data[] =
  {
    0x23, 0x23, 0x53, 0x43, 0x30, 0x30, 0x31, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x78, 0x9c, 0x03, 0x00, 0x00, 0x00, 0x00, 0x01
  };

  WriteOneByOne( inflateStream, DataRef( data, data + sizeof( data ) ) );
  inflateStream.End();

  EXPECT_EQ( "", stringStream.output );
  EXPECT_TRUE( stringStream.ended );
}
