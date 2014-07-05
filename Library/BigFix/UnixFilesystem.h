#ifndef BigFix_UnixFilesystem_h
#define BigFix_UnixFilesystem_h

#include "Filesystem.h"
#include <dirent.h>

namespace BigFix
{

class UnixFile : public File
{
public:
  UnixFile( int fd, const std::string& path );
  virtual ~UnixFile();

  virtual void SetModificationTime( const DateTime& );

  virtual size_t Read( uint8_t* buffer, size_t length );
  virtual void Write( DataRef );

private:
  int m_fd;
  std::string m_path;
};

class OpenDir
{
public:
  explicit OpenDir( const char* path );
  ~OpenDir();

  operator DIR*() const { return m_dir; }

private:
  OpenDir( const OpenDir& );
  OpenDir& operator=( const OpenDir& );

  DIR* m_dir;
};

}

#endif
