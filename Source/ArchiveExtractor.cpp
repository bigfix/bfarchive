#include "ArchiveExtractor.h"
#include "BigFix/DataRef.h"
#include "BigFix/Error.h"

using namespace BigFix;

void ArchiveExtractor::Directory( const char* name,
                                  const DateTime& mtime )
{
  throw Error( "directory extraction not supported yet" );
}

BigFix::Stream& ArchiveExtractor::File( const char* name,
                                        const DateTime& mtime,
                                        uint64_t length )
{
  m_fileStream.Reset( OpenNewFile( name ) );
  return m_fileStream;
}

void ArchiveExtractor::End()
{
}
