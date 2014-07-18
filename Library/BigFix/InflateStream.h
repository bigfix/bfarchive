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

#ifndef BigFix_InflateStream_h
#define BigFix_InflateStream_h

#include "BigFix/Stream.h"
#include <stdint.h>
#include <zlib.h>

namespace BigFix
{

class InflateStream : public Stream
{
public:
  explicit InflateStream( Stream& output );
  virtual ~InflateStream();

  virtual void Write( DataRef );
  virtual void End();

private:
  const uint8_t* Header( const uint8_t* start, const uint8_t* end );
  const uint8_t* Checksum( const uint8_t* start, const uint8_t* end );
  const uint8_t* Compressed( const uint8_t* start, const uint8_t* end );

  enum State { STATE_HEADER, STATE_CHECKSUM, STATE_COMPRESSED, STATE_RAW };

  Stream& m_output;
  State m_state;
  z_stream m_zstream;
  int m_zstatus;
  size_t m_pos;
  uint8_t m_header[8];
};

}

#endif
