#include "ArchiveCreator.h"
#include "BigFix/DateTime.h"
#include "BigFix/Filesystem.h"
#include <iostream>

using namespace BigFix;

ArchiveCreator::ArchiveCreator( Stream& output, bool verbose )
  : m_writer( output )
  , m_verbose( verbose )
{
}

void ArchiveCreator::Create( const std::string& path )
{
  RecursivelyAddDir( path, "" );
  m_writer.End();
}

static std::string JoinArchivePath( const std::string& parent,
                                    const std::string& child )
{
  if ( parent.empty() || parent == "." )
    return child;

  return parent + "/" + child;
}

void ArchiveCreator::RecursivelyAddDir( const std::string& filePath,
                                        const std::string& archivePath )
{
  std::vector<std::string> entries = ReadDir( filePath.c_str() );

  for ( std::vector<std::string>::const_iterator it = entries.begin(),
                                                 end = entries.end();
        it != end;
        it++ )
  {
    std::string relativeFilePath = JoinFilePath( filePath, *it );
    std::string relativeArchivePath = JoinArchivePath( archivePath, *it );

    FileStatus status = Stat( relativeFilePath.c_str() );

    if ( status.IsDirectory() )
    {
      if ( m_verbose )
        std::cout << relativeArchivePath << std::endl;

      m_writer.Directory( relativeArchivePath.c_str(),
                          status.ModificationTime() );

      RecursivelyAddDir( relativeFilePath, relativeArchivePath );
    }
    else if ( status.IsFile() )
    {
      if ( m_verbose )
        std::cout << relativeArchivePath << std::endl;

      Stream& contentStream = m_writer.File( relativeArchivePath.c_str(),
                                             status.ModificationTime(),
                                             status.Length() );

      StreamFile( relativeFilePath.c_str(), contentStream );
    }
  }
}
