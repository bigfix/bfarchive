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
