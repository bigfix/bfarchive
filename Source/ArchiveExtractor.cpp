#include "ArchiveExtractor.h"
#include "BigFix/DataRef.h"
#include "BigFix/Error.h"
#include <iostream>

using namespace BigFix;

ArchiveExtractor::ArchiveExtractor( const char* outputDir, bool verbose )
  : m_outputDir( outputDir ), m_verbose( verbose )
{
}

void ArchiveExtractor::Directory( const char* path,
                                  const DateTime& mtime )
{
  if ( m_verbose )
    std::cout << path << std::endl;

  MakeDir( path );
}

BigFix::Stream& ArchiveExtractor::File( const char* path,
                                        const DateTime& mtime,
                                        uint64_t length )
{
  if ( m_verbose )
    std::cout << path << std::endl;

  std::string fullPath = m_outputDir + "/" + path;
  m_fileStream.Reset( OpenNewFile( fullPath.c_str() ) );
  return m_fileStream;
}

void ArchiveExtractor::End()
{
}
