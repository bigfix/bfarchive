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

#ifndef TestUtility_h
#define TestUtility_h

#include "BigFix/Stream.h"
#include <stdint.h>
#include <string>
#include <vector>

class VectorStream : public BigFix::Stream
{
public:
  explicit VectorStream( std::vector<uint8_t>& );

  virtual void Write( BigFix::DataRef data );
  virtual void End();

  std::vector<uint8_t>& output;
  bool ended;
};

class StringStream : public BigFix::Stream
{
public:
  StringStream();
  explicit StringStream( std::string& );

  virtual void Write( BigFix::DataRef data );
  virtual void End();

  std::string& output;
  bool ended;
};

void WriteOneByOneAndEnd( BigFix::Stream& stream, BigFix::DataRef data );

std::string Sandbox( const std::string& path );

#endif
