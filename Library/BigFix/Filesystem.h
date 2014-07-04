#ifndef BigFix_Filesystem_h
#define BigFix_Filesystem_h

#include "BigFix/Encoding.h" 
#include <memory>

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

std::auto_ptr<File> OpenNewFile( const char* name, Encoding nameEncoding );
std::auto_ptr<File> OpenExistingFile( const char* name, Encoding nameEncoding );

void MakeDir( const char* name, Encoding nameEncoding );

}

#endif
