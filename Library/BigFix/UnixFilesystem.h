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

#ifndef BigFix_UnixFilesystem_h
#define BigFix_UnixFilesystem_h

#include "Filesystem.h"
#include <dirent.h>

namespace BigFix
{

class UnixFile : public File
{
public:
  UnixFile( int fd, const std::string& path );
  virtual ~UnixFile();

  virtual void SetModificationTime( const DateTime& );

  virtual size_t Read( uint8_t* buffer, size_t length );
  virtual void Write( DataRef );

private:
  int m_fd;
  std::string m_path;
};

class OpenDir
{
public:
  explicit OpenDir( const char* path );
  ~OpenDir();

  operator DIR*() const { return m_dir; }

private:
  OpenDir( const OpenDir& );
  OpenDir& operator=( const OpenDir& );

  DIR* m_dir;
};

}

#endif
