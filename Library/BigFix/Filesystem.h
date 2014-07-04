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

  virtual void Write( DataRef ) = 0;
};

std::auto_ptr<File> OpenNewFile( const char* name, Encoding nameEncoding );

}

#endif
