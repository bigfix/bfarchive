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

#ifndef BigFix_WindowsFilesystem_h
#define BigFix_WindowsFilesystem_h

#include "BigFix/Filesystem.h"
#include <windows.h>

namespace BigFix
{

class WindowsFile : public File
{
public:
  explicit WindowsFile( HANDLE handle );
  virtual ~WindowsFile();

  virtual void SetModificationTime( const DateTime& );

  virtual size_t Read( uint8_t* buffer, size_t length );
  virtual void Write( DataRef );

private:
  HANDLE m_handle;
};

class FindHandle
{
public:
  explicit FindHandle( HANDLE );
  ~FindHandle();

  operator HANDLE() const { return m_handle; }

private:
  FindHandle( const FindHandle& );
  FindHandle& operator=( const FindHandle& );

  HANDLE m_handle;
};

std::string MakePortableFilePath( const wchar_t* path );

}

#endif
