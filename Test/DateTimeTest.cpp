#include "BigFix/DataRef.h"
#include "BigFix/DateTime.h"
#include "BigFix/Error.h"
#include <gtest/gtest.h>

using namespace BigFix;

TEST( DateTimeTest, DefaultIsTheEpoch )
{
  DateTime time;

  EXPECT_EQ( 1970, time.Year() );
  EXPECT_EQ( 1, time.Month() );
  EXPECT_EQ( 1, time.Day() );
  EXPECT_EQ( 0, time.Hour() );
  EXPECT_EQ( 0, time.Minute() );
  EXPECT_EQ( 0, time.Second() );
}

TEST( DateTimeTest, FromString )
{
  DateTime time( DataRef( "Sun, 11 Mar 1984 08:23:42 +0000" ) );

  EXPECT_EQ( 1984, time.Year() );
  EXPECT_EQ( 3, time.Month() );
  EXPECT_EQ( 11, time.Day() );
  EXPECT_EQ( 8, time.Hour() );
  EXPECT_EQ( 23, time.Minute() );
  EXPECT_EQ( 42, time.Second() );
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
