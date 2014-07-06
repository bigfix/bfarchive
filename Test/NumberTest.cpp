#include "BigFix/Error.h"
#include "BigFix/DataRef.h"
#include "BigFix/Number.h"
#include <gtest/gtest.h>

using namespace BigFix;

TEST( NumberTest, Read4ByteIntger )
{
  uint8_t number[4] = { 0x01, 0x02, 0x03, 0x04 };

  EXPECT_EQ( 67305985ull, ReadLittleEndian( DataRef( number, number + 4 ) ) );
}

TEST( NumberTest, Read8ByteIntger )
{
  uint8_t number[8] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08 };

  EXPECT_EQ( 578437695752307201ull,
             ReadLittleEndian( DataRef( number, number + 8 ) ) );
}

TEST( NumberTest, Write4ByteInteger )
{
  uint8_t number[4];
  WriteLittleEndian( 67305985, number, 4 );

  EXPECT_EQ( 1, number[0] );
  EXPECT_EQ( 2, number[1] );
  EXPECT_EQ( 3, number[2] );
  EXPECT_EQ( 4, number[3] );
}

TEST( NumberTest, Write8ByteInteger )
{
  uint8_t number[8];
  WriteLittleEndian( 578437695752307201, number, 8 );

  EXPECT_EQ( 1, number[0] );
  EXPECT_EQ( 2, number[1] );
  EXPECT_EQ( 3, number[2] );
  EXPECT_EQ( 4, number[3] );
  EXPECT_EQ( 5, number[4] );
  EXPECT_EQ( 6, number[5] );
  EXPECT_EQ( 7, number[6] );
  EXPECT_EQ( 8, number[7] );
}

TEST( NumberTest, ReadAsciiNumber )
{
  EXPECT_EQ( 42ul, ReadAsciiNumber<uint8_t>( DataRef( "42" ) ) );
  EXPECT_EQ( 1970, ReadAsciiNumber<int32_t>( DataRef( "1970" ) ) );
  EXPECT_THROW( ReadAsciiNumber<uint8_t>( DataRef( "hello" ) ), Error );
}
