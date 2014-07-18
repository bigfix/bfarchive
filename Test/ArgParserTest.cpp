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

#include "BigFix/ArgParser.h"
#include "BigFix/Error.h"
#include <gtest/gtest.h>

using namespace BigFix;

TEST( ArgParserTest, ThrowsOnUnknownLongArgs )
{
  ArgParser parser;
  const char* args[] = { "test", "--hello" };
  EXPECT_THROW( parser.Parse( 2, args ), Error );
}

TEST( ArgParserTest, ThrowsOnUnknownShortArgs )
{
  ArgParser parser;
  const char* args[] = { "test", "-h" };
  EXPECT_THROW( parser.Parse( 2, args ), Error );
}

TEST( ArgParserTest, DashIsAnArg )
{
  ArgParser parser;
  const char* args[] = { "test", "-" };
  parser.Parse( 2, args );

  ASSERT_EQ( 1ul, parser.Arguments().size() );
  ASSERT_EQ( "-", parser.Arguments()[0] );
}

TEST( ArgParserTest, ShortArgsTest )
{
  ArgParser parser;
  parser.AddOption( 'h', "hello" );
  parser.AddOption( 'g', "goodbye" );

  const char* args[] = { "test", "-hg", "foo" };
  parser.Parse( 3, args );

  EXPECT_TRUE( parser.HasOption( 'h' ) );
  EXPECT_TRUE( parser.HasOption( "hello" ) );

  EXPECT_TRUE( parser.HasOption( 'g' ) );
  EXPECT_TRUE( parser.HasOption( "goodbye" ) );

  ASSERT_EQ( 1ul, parser.Arguments().size() );
  ASSERT_EQ( "foo", parser.Arguments()[0] );
}

TEST( ArgParserTest, LongArgsTest )
{
  ArgParser parser;
  parser.AddOption( 'h', "hello" );
  parser.AddOption( 'g', "goodbye" );

  const char* args[] = { "test", "--hello", "--goodbye", "foo" };
  parser.Parse( 4, args );

  EXPECT_TRUE( parser.HasOption( 'h' ) );
  EXPECT_TRUE( parser.HasOption( "hello" ) );

  EXPECT_TRUE( parser.HasOption( 'g' ) );
  EXPECT_TRUE( parser.HasOption( "goodbye" ) );

  ASSERT_EQ( 1ul, parser.Arguments().size() );
  ASSERT_EQ( "foo", parser.Arguments()[0] );
}
