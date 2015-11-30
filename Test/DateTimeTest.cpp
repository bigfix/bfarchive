/*
  Copyright 2014 International Business Machines, Inc.

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
*/

#include "BigFix/DataRef.h"
#include "BigFix/DateTime.h"
#include "BigFix/Error.h"
#include "BigFix/TestSeams.h"
#include "ScopedMock.h"
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

static int snprintfError( char* buffer,
                          size_t size,
                          const char* format,
                          const char* dayOfWeek,
                          int day,
                          const char* month,
                          int year,
                          int hour,
                          int minute,
                          int second )
{
  return 0;
}

TEST( DateTimeTest, ToStingFails )
{
  ScopedMock<Type_snprintf> guard( snprintfError, Real_snprintf, Set_snprintf );

  DateTime date( DataRef( "Sun, 11 Mar 1984 08:23:42 +0000" ) );

  try
  {
    date.ToString();
    FAIL();
  }
  catch ( const std::exception& err )
  {
    std::string message = err.what();
    std::string expected = "Failed to convert date to string";
    EXPECT_EQ( expected, message );
  }
  catch ( ... ) { FAIL(); }
}
