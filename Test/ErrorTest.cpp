#include "BigFix/Error.h"
#include <gtest/gtest.h>

using namespace BigFix;

TEST( ErrorTest, ErrorIsAStdException )
{
  try
  {
    throw Error( "Oh noes!" );
  }
  catch ( const std::exception& e )
  {
    EXPECT_EQ( std::string( "Oh noes!" ), e.what() );
  }
  catch ( ... )
  {
    FAIL();
  }
}
