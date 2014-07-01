#include "BigFix/DataRef.h"
#include "BigFix/Error.h"
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

TEST( DataRefTest, Subscript )
{
  DataRef hello( "hello" );

  EXPECT_EQ( 'h', hello[0] );
  EXPECT_EQ( 'o', hello[4] );
  EXPECT_THROW( hello[5], Error );
}

TEST( DataRefTest, Slice )
{
  DataRef hello( "hello" );

  DataRef hel = hello.Slice( 0, 3 );
  DataRef lo = hello.Slice( 3, 2 );

  EXPECT_EQ( 3, hel.Length() );
  EXPECT_TRUE( memcmp( hel.Start(), "hel", 3 ) == 0 );

  EXPECT_EQ( 2, lo.Length() );
  EXPECT_TRUE( memcmp( lo.Start(), "lo", 2 ) == 0 );
}

TEST( DataRefTest, InvalidSlice )
{
  DataRef hello( "hello" );

  EXPECT_THROW( hello.Slice( 0, 6 ), Error );
  EXPECT_THROW( hello.Slice( 1, 5 ), Error );
  EXPECT_THROW( hello.Slice( 5, 1 ), Error );
}

TEST( DataRefTest, Equals )
{
  EXPECT_TRUE( DataRef() == DataRef() );
  EXPECT_TRUE( DataRef( "hello" ) == DataRef( "hello" ) );
  EXPECT_FALSE( DataRef( "hello" ) == DataRef( "world" ) );
}
