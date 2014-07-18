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

#ifndef ArchiveCreator_h
#define ArchiveCreator_h

#include "BigFix/ArchiveWriter.h"
#include <string>

namespace BigFix
{
  class FileStatus;
}

class ArchiveCreator
{
public:
  ArchiveCreator( BigFix::Stream& output, bool verbose );

  void Create( const std::string& path );

private:
  void RecursivelyAddDir( const std::string& filePath,
                          const std::string& archivePath );

  void AddFile( const BigFix::FileStatus&,
                const std::string& filePath,
                const std::string& archivePath );

  BigFix::ArchiveWriter m_writer;
  bool m_verbose;
};

#endif
