#include "Stream.h"
#include "BigFix/DataRef.h"

namespace BigFix
{

Stream::~Stream()
{
}

void NullStream::Write( DataRef )
{
}

void NullStream::End()
{
}

AppendStream::AppendStream() : m_output( 0 )
{
}

void AppendStream::Reset( Stream& output )
{
  m_output = &output;
}

void AppendStream::Write( DataRef data )
{
  m_output->Write( data );
}

void AppendStream::End()
{
  m_output = 0;
}

void WriteAndEnd( Stream& stream, DataRef data )
{
  stream.Write( data );
  stream.End();
}

}
