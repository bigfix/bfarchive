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

#include "ArchiveLister.h"
#include <iostream>

void ArchiveLister::Directory( const char* path, const BigFix::DateTime& mtime )
{
  std::cout << path << std::endl;
}

BigFix::Stream& ArchiveLister::File( const char* path,
                                     const BigFix::DateTime& mtime,
                                     uint64_t length )
{
  std::cout << path << std::endl;
  return m_ignore;
}

void ArchiveLister::End()
{
}
