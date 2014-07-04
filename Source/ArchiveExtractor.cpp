#include "ArchiveExtractor.h"
#include "BigFix/DataRef.h"
#include "BigFix/Error.h"

using namespace BigFix;

void ArchiveExtractor::Directory( const char* name,
                                  Encoding nameEncoding,
                                  const DateTime& mtime )
{
  throw Error( "directory extraction not supported yet" );
}

void ArchiveExtractor::FileStart( const char* name,
                                  Encoding nameEncoding,
                                  const DateTime& mtime,
                                  uint64_t length )
{
  m_currentFile = OpenNewFile( name, nameEncoding );
}

void ArchiveExtractor::FileWrite( DataRef data )
{
  if ( !m_currentFile.get() )
    throw Error( "Internal error (writing data to file before starting it)" );

  m_currentFile->Write( data );
}

void ArchiveExtractor::FileEnd()
{
  if ( !m_currentFile.get() )
    throw Error( "Internal error (closing file before starting it)" );

  m_currentFile.reset();
}

void ArchiveExtractor::End()
{
}
