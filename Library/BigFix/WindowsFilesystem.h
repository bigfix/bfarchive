#ifndef BigFix_WindowsFilesystem_h
#define BigFix_WindowsFilesystem_h

#include "BigFix/Filesystem.h"
#include <windows.h>

namespace BigFix
{

class WindowsFile : public File
{
public:
  explicit WindowsFile( HANDLE handle );
  virtual ~WindowsFile();

  virtual void SetModificationTime( const DateTime& );

  virtual size_t Read( uint8_t* buffer, size_t length );
  virtual void Write( DataRef );

private:
  HANDLE m_handle;
};

class FindHandle
{
public:
  explicit FindHandle( HANDLE );
  ~FindHandle();

  operator HANDLE() const { return m_handle; }

private:
  FindHandle( const FindHandle& );
  FindHandle& operator=( const FindHandle& );

  HANDLE m_handle;
};

std::string MakePortableFilePath( const wchar_t* path );

}

#endif
