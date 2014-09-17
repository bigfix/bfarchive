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

#include "ArchiveExtractor.h"
#include "BigFix/DataRef.h"
#include "BigFix/Error.h"
#include <iostream>

using namespace BigFix;

void ArchiveExtractorFileStream::Reset( std::auto_ptr<File> file,
                                        const DateTime& mtime )
{
  FileStream::Reset( file );
  m_mtime = mtime;
}

void ArchiveExtractorFileStream::End()
{
  m_file->SetModificationTime( m_mtime );
  FileStream::End();
}

ArchiveExtractor::ArchiveExtractor( const std::string& outputDir, bool verbose )
  : m_outputDir( outputDir ), m_verbose( verbose )
{
}

void ArchiveExtractor::Directory( const char* path,
                                  const DateTime& mtime )
{
  if ( m_verbose )
    std::cout << path << std::endl;

  std::string fullPath = JoinPath( m_outputDir, path );
  MakeDir( fullPath.c_str() );
}

BigFix::Stream& ArchiveExtractor::File( const char* path,
                                        const DateTime& mtime,
                                        uint64_t length )
{
  if ( m_verbose )
    std::cout << path << std::endl;

  std::string fullPath = JoinPath( m_outputDir, path );
  m_fileStream.Reset( OpenAsNewFile( fullPath.c_str() ), mtime );
  return m_fileStream;
}

void ArchiveExtractor::End()
{
}
