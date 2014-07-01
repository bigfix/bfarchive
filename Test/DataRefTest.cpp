#include "BigFix/DataRef.h"
#include <gtest/gtest.h>

using namespace BigFix;

TEST( DataRefTest, ConstructNothing )
{
  DataRef empty;

  EXPECT_EQ( 0, empty.Length() );
  EXPECT_TRUE( empty.IsEmpty() );
  EXPECT_EQ( 0, empty.Start() );
  EXPECT_EQ( 0, empty.End() );
}

TEST( DataRefTest, ConstructLiteral )
{
  DataRef hello( "hello" );

  EXPECT_EQ( 5, hello.Length() );
  EXPECT_FALSE( hello.IsEmpty() );
  EXPECT_TRUE( memcmp( hello.Start(), "hello", 5 ) == 0 );
  EXPECT_EQ( hello.Start() + 5, hello.End() );
}

TEST( DataRefTest, ConstructString )
{
  std::string helloString( "hello" );
  DataRef hello( helloString );

  EXPECT_EQ( 5, hello.Length() );
  EXPECT_FALSE( hello.IsEmpty() );
  EXPECT_TRUE( memcmp( hello.Start(), "hello", 5 ) == 0 );
  EXPECT_EQ( hello.Start() + 5, hello.End() );
}

TEST( DataRefTest, ConstructPointers )
{
  const uint8_t helloBuffer[] = { 'h', 'e', 'l', 'l', 'o' };

  DataRef hello( helloBuffer, helloBuffer + sizeof( helloBuffer ) );

  EXPECT_EQ( 5, hello.Length() );
  EXPECT_FALSE( hello.IsEmpty() );
  EXPECT_TRUE( memcmp( hello.Start(), "hello", 5 ) == 0 );
  EXPECT_EQ( hello.Start() + 5, hello.End() );
}
