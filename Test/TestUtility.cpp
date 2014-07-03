#include "TestUtility.h"
#include "BigFix/DataRef.h"

using namespace BigFix;

VectorStream::VectorStream() : ended( false )
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

StringStream::StringStream() : ended( false )
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

void WriteOneByOne( Stream& stream, DataRef data )
{
  for ( const uint8_t* it = data.Start(); it != data.End(); it++ )
    stream.Write( DataRef( it, it + 1 ) );
}
