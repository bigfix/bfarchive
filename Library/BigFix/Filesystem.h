#ifndef BigFix_Filesystem_h
#define BigFix_Filesystem_h

#include "BigFix/Stream.h"
#include <memory>
#include <stddef.h>
#include <stdint.h>

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

std::auto_ptr<File> OpenNewFile( const char* name );
std::auto_ptr<File> OpenExistingFile( const char* name );

void MakeDir( const char* name );

}

#endif
