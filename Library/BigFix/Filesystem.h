#ifndef BigFix_Filesystem_h
#define BigFix_Filesystem_h

#include "BigFix/DateTime.h"
#include "BigFix/Stream.h"
#include <memory>
#include <stddef.h>
#include <stdint.h>
#include <string>
#include <vector>

namespace BigFix
{

class DataRef;

class File
{
public:
  virtual ~File();

  virtual void SetModificationTime( const DateTime& ) = 0;

  virtual size_t Read( uint8_t* buffer, size_t length ) = 0;
  virtual void Write( DataRef ) = 0;
};

class FileStream : public Stream
{
public:
  void Reset( std::auto_ptr<File> );

  virtual void Write( DataRef );
  virtual void End();

protected:
  std::auto_ptr<File> m_file;
};

class FileStatus
{
public:
  FileStatus( uint64_t length,
              const DateTime& mtime,
              bool isDirectory,
              bool isFile );

  uint64_t Length() const { return m_length; }
  const DateTime& ModificationTime() const { return m_mtime; }
  bool IsDirectory() const { return m_isDirectory; }
  bool IsFile() const { return m_isFile; }

private:
  uint64_t m_length;
  DateTime m_mtime;
  bool m_isDirectory;
  bool m_isFile;
};

std::auto_ptr<File> OpenNewFile( const char* path );
std::auto_ptr<File> OpenExistingFile( const char* path );

void MakeDir( const char* path );

FileStatus Stat( const char* path );

std::string JoinFilePath( const std::string& parent, const std::string& child );

void StreamStdIn( Stream& );
void StreamFile( const char* path, Stream& );
std::vector<std::string> ReadDir( const char* path );

bool IsAscii( const char* path );

std::string LocalPathToUTF8Path( const char* path );

}

#endif
