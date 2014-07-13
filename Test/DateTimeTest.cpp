#include "BigFix/DataRef.h"
#include "BigFix/DateTime.h"
#include "BigFix/Error.h"
#include <gtest/gtest.h>

using namespace BigFix;

TEST( DateTimeTest, DefaultIsTheEpoch )
{
  DateTime time;

  EXPECT_EQ( 1970ul, time.Year() );
  EXPECT_EQ( 1ul, time.Month() );
  EXPECT_EQ( 1ul, time.Day() );
  EXPECT_EQ( 0ul, time.Hour() );
  EXPECT_EQ( 0ul, time.Minute() );
  EXPECT_EQ( 0ul, time.Second() );
}

TEST( DateTimeTest, FromString )
{
  DateTime time( DataRef( "Sun, 11 Mar 1984 08:23:42 +0000" ) );

  EXPECT_EQ( 1984ul, time.Year() );
  EXPECT_EQ( 3ul, time.Month() );
  EXPECT_EQ( 11ul, time.Day() );
  EXPECT_EQ( 8ul, time.Hour() );
  EXPECT_EQ( 23ul, time.Minute() );
  EXPECT_EQ( 42ul, time.Second() );
}

TEST( DateTimeTest, ToString )
{
  EXPECT_EQ(
    "Sun, 11 Mar 1984 08:23:42 +0000",
    DateTime( DataRef( "Sun, 11 Mar 1984 08:23:42 +0000" ) ).ToString() );
}

TEST( DateTimeTest, FromConstructor )
{
  EXPECT_EQ(
    "Sun, 11 Mar 1984 08:23:42 +0000",
    DateTime( 1984, 3, 11, 1, 8, 23, 42 ).ToString() );
}

TEST( DateTimeTest, ThrowsOnInvalidDates )
{
  DataRef badLength( "hodor" );
  DataRef badDay( "Xxx, 11 Mar 1984 08:23:42 +0000" );
  DataRef badMonth( "Sun, 11 Xxx 1984 08:23:42 +0000" );

  EXPECT_THROW( DateTime date( badLength ), Error );
  EXPECT_THROW( DateTime date( badDay ), Error );
  EXPECT_THROW( DateTime date( badMonth ), Error );
}
