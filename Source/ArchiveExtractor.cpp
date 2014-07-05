#include "ArchiveExtractor.h"
#include "BigFix/DataRef.h"
#include "BigFix/Error.h"

using namespace BigFix;

ArchiveExtractor::ArchiveExtractor( const char* outputDir )
  : m_outputDir( outputDir )
{
}

void ArchiveExtractor::Directory( const char* path,
                                  const DateTime& mtime )
{
  throw Error( "directory extraction not supported yet" );
}

BigFix::Stream& ArchiveExtractor::File( const char* path,
                                        const DateTime& mtime,
                                        uint64_t length )
{
  std::string fullPath = m_outputDir + "/" + path;
  m_fileStream.Reset( OpenNewFile( fullPath.c_str() ) );
  return m_fileStream;
}

void ArchiveExtractor::End()
{
}
