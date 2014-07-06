#include "ArchiveCreator.h"
#include "BigFix/DateTime.h"
#include "BigFix/Error.h"
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
  FileStatus status = Stat( path.c_str() );

  if ( status.IsDirectory() )
  {
    RecursivelyAddDir( path, "" );
  }
  else if ( status.IsFile() )
  {
    AddFile( status, path, path );
  }
  else
  {
    throw Error( "Archives can only be created from directories or files" );
  }

  m_writer.End();
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
    std::string relativeFilePath = JoinPath( filePath, *it );
    std::string relativeArchivePath = JoinPath( archivePath, *it );

    FileStatus status = Stat( relativeFilePath.c_str() );

    if ( status.IsDirectory() )
    {
      if ( m_verbose )
        std::cout << relativeArchivePath << "/" << std::endl;

      m_writer.Directory( relativeArchivePath.c_str(),
                          status.ModificationTime() );

      RecursivelyAddDir( relativeFilePath, relativeArchivePath );
    }
    else if ( status.IsFile() )
    {
      AddFile( status, relativeFilePath, relativeArchivePath );
    }
  }
}

void ArchiveCreator::AddFile( const FileStatus& status,
                              const std::string& filePath,
                              const std::string& archivePath )
{
  if ( m_verbose )
    std::cout << archivePath << std::endl;

  Stream& contentStream = m_writer.File( archivePath.c_str(),
                                         status.ModificationTime(),
                                         status.Length() );

  StreamFile( filePath.c_str(), contentStream );
}
