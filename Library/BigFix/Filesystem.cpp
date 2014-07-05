#include "Filesystem.h"
#include "BigFix/DataRef.h"

namespace BigFix
{

File::~File()
{
}

void FileStream::Reset( std::auto_ptr<File> file )
{
  m_file = file;
}

void FileStream::Write( DataRef data )
{
  m_file->Write( data );
}

void FileStream::End()
{
  m_file.reset();
}

}
