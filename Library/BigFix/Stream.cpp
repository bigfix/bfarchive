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
