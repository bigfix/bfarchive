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

#ifndef ArchiveExtractor_h
#define ArchiveExtractor_h

#include "BigFix/ArchiveStream.h"
#include "BigFix/Filesystem.h"
#include <string>

class ArchiveExtractorFileStream : public BigFix::FileStream
{
public:
  void Reset( std::auto_ptr<BigFix::File>, const BigFix::DateTime& mtime );

  virtual void End();

private:
  BigFix::DateTime m_mtime;
};

class ArchiveExtractor : public BigFix::ArchiveStream
{
public:
  explicit ArchiveExtractor( const std::string& outputDir, bool verbose );

  virtual void Directory( const char* path,
                          const BigFix::DateTime& mtime );

  virtual BigFix::Stream& File( const char* path,
                                const BigFix::DateTime& mtime,
                                uint64_t length );

  virtual void End();

private:
  std::string m_outputDir;
  bool m_verbose;
  ArchiveExtractorFileStream m_fileStream;
};

#endif
