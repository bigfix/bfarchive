#include "BigFix/DataRef.h"
#include "BigFix/DeflateStream.h"
#include "TestUtility.h"
#include <gtest/gtest.h>

using namespace BigFix;

TEST( DeflateStreamTest, CompressEmpty )
{
  VectorStream vectorStream;
  DeflateStream deflateStream( vectorStream );

  deflateStream.End();

  uint8_t expected[] =
  {
    0x23, 0x23, 0x53, 0x43, 0x30, 0x30, 0x31, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x78, 0x9c, 0x03, 0x00, 0x00, 0x00, 0x00, 0x01
  };

  ASSERT_EQ( sizeof( expected ), vectorStream.output.size() );
  EXPECT_TRUE( std::equal(
    vectorStream.output.begin(), vectorStream.output.end(), expected ) );

  EXPECT_TRUE( vectorStream.ended );
}

TEST( DeflateStreamTest, CompressHelloWorld )
{
  VectorStream vectorStream;
  DeflateStream deflateStream( vectorStream );

  WriteOneByOne( deflateStream, DataRef( "Hello, world!" ) );
  deflateStream.End();

  uint8_t expected[] =
  {
    0x23, 0x23, 0x53, 0x43, 0x30, 0x30, 0x31, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x78, 0x9c, 0xf3, 0x48, 0xcd, 0xc9, 0xc9, 0xd7, 0x51, 0x28, 0xcf, 0x2f,
    0xca, 0x49, 0x51, 0x04, 0x00, 0x20, 0x5e, 0x04, 0x8a
  };

  ASSERT_EQ( sizeof( expected ), vectorStream.output.size() );
  EXPECT_TRUE( std::equal(
    vectorStream.output.begin(), vectorStream.output.end(), expected ) );

  EXPECT_TRUE( vectorStream.ended );
}
