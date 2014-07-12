#include "TestUtility.h"
#include "BigFix/DataRef.h"

using namespace BigFix;

VectorStream::VectorStream( std::vector<uint8_t>& output )
  : output( output ), ended( false )
{
}

void VectorStream::Write( DataRef data )
{
  output.insert( output.end(), data.Start(), data.End() );
}

void VectorStream::End()
{
  ended = true;
}

StringStream::StringStream( std::string& output )
  : output( output ), ended( false )
{
}

void StringStream::Write( DataRef data )
{
  output.insert( output.end(), data.Start(), data.End() );
}

void StringStream::End()
{
  ended = true;
}

void WriteOneByOneAndEnd( Stream& stream, DataRef data )
{
  for ( const uint8_t* it = data.Start(); it != data.End(); it++ )
  {
    stream.Write( DataRef( it, it + 1 ) );
    stream.Write( DataRef() );
  }

  stream.End();
}
