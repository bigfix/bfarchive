#ifndef BigFix_Filesystem_h
#define BigFix_Filesystem_h

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

  virtual size_t Read( uint8_t* buffer, size_t length ) = 0;
  virtual void Write( DataRef ) = 0;
};

class FileStream : public Stream
{
public:
  void Reset( std::auto_ptr<File> );

  virtual void Write( DataRef );
  virtual void End();

private:
  std::auto_ptr<File> m_file;
};

class DirectoryEntry
{
public:
  DirectoryEntry( const std::string& path,
                  uint64_t length,
                  bool isDirectory,
                  bool isFile );

  const std::string& Path() const { return m_path; }
  uint64_t Length() const { return m_length; }

  bool IsDirectory() const { return m_isDirectory; }
  bool IsFile() const { return m_isFile; }

private:
  std::string m_path;
  uint64_t m_length;
  bool m_isDirectory;
  bool m_isFile;
};

std::auto_ptr<File> OpenNewFile( const char* path );
std::auto_ptr<File> OpenExistingFile( const char* path );

void MakeDir( const char* path );

void ReadStdIn( Stream& );
void ReadFile( const char* path, Stream& );

std::vector<DirectoryEntry> ReadDir( const char* path );

}

#endif
