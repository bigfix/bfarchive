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

#ifndef BigFix_ArchiveReader_h
#define BigFix_ArchiveReader_h

#include "BigFix/ArchiveStream.h"
#include "BigFix/DateTime.h"
#include "BigFix/Stream.h"
#include <stdint.h>
#include <time.h>

namespace BigFix
{

class ArchiveReader : public Stream
{
public:
  explicit ArchiveReader( ArchiveStream& output );

  virtual void Write( DataRef );
  virtual void End();

private:
  const uint8_t* PathEncoding( const uint8_t* start, const uint8_t* end );
  const uint8_t* FileLengthLength( const uint8_t* start, const uint8_t* end );
  const uint8_t* PathLength( const uint8_t* start, const uint8_t* end );
  const uint8_t* Path( const uint8_t* start, const uint8_t* end );
  const uint8_t* DateLength( const uint8_t* start, const uint8_t* end );
  const uint8_t* Date( const uint8_t* start, const uint8_t* end );
  const uint8_t* FileLength( const uint8_t* start, const uint8_t* end );
  const uint8_t* File( const uint8_t* start, const uint8_t* end );

  enum State
  {
    STATE_PATH_ENCODING,
    STATE_FILE_LENGTH_LENGTH,
    STATE_PATH_LENGTH,
    STATE_PATH,
    STATE_DATE_LENGTH,
    STATE_DATE,
    STATE_FILE_LENGTH,
    STATE_FILE,
    STATE_DONE
  };

  ArchiveStream& m_output;
  State m_state;
  size_t m_fileLengthLength;
  bool m_pathIsUTF8;
  bool m_isDirectory;
  DateTime m_mtime;
  size_t m_pos;
  size_t m_length;
  uint64_t m_filePos;
  uint64_t m_fileLength;
  Stream* m_fileStream;
  uint8_t m_path[256];
  uint8_t m_buffer[256];
};

}

#endif
