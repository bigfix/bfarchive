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

  ASSERT_EQ( 1, parser.Arguments().size() );
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

  ASSERT_EQ( 1, parser.Arguments().size() );
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

  ASSERT_EQ( 1, parser.Arguments().size() );
  ASSERT_EQ( "foo", parser.Arguments()[0] );
}
