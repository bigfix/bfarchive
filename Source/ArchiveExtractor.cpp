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

  std::string fullPath = JoinFilePath( m_outputDir, path );
  MakeDir( fullPath.c_str() );
}

BigFix::Stream& ArchiveExtractor::File( const char* path,
                                        const DateTime& mtime,
                                        uint64_t length )
{
  if ( m_verbose )
    std::cout << path << std::endl;

  std::string fullPath = JoinFilePath( m_outputDir, path );
  m_fileStream.Reset( OpenNewFile( fullPath.c_str() ), mtime );
  return m_fileStream;
}

void ArchiveExtractor::End()
{
}
