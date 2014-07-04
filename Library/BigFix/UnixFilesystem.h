#ifndef BigFix_UnixFilesystem_h
#define BigFix_UnixFilesystem_h

#include "Filesystem.h"

namespace BigFix
{

class UnixFile : public File
{
public:
  explicit UnixFile( int fd );
  virtual ~UnixFile();

  virtual size_t Read( uint8_t* buffer, size_t length );
  virtual void Write( DataRef );

private:
  int m_fd;
};

}

#endif
