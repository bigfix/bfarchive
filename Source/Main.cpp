#include "BigFix/ArchiveReader.h"
#include "BigFix/ArchiveStream.h"
#include "BigFix/DataRef.h"
#include "BigFix/InflateStream.h"
#include <iostream>

using namespace BigFix;

class ArchiveListStream : public ArchiveStream
{
public:
  virtual void Directory( const char* name,
                          ArchiveEncoding nameEncoding,
                          const DateTime& mtime )
  {
    std::cout << name << std::endl;
  }

  virtual void FileStart( const char* name,
                          ArchiveEncoding nameEncoding,
                          const DateTime& mtime,
                          uint64_t length )
  {
    std::cout << name << std::endl;
  }

  virtual void FileWrite( const uint8_t* start, const uint8_t* end ) {}

  virtual void FileEnd() {}
};

int main()
{
  ArchiveListStream list;
  ArchiveReader reader( list );
  InflateStream inflate( reader );

  char buffer[4096];

  while ( true )
  {
    ssize_t nread = read( 0, buffer, sizeof( buffer ) );
    if ( nread == 0 )
    {
      inflate.End();
      return 0;
    }

    if ( nread < 0 )
    {
      std::cerr << "Error reading from stdin" << std::endl;
      return 1;
    }

    inflate.Write( DataRef( (uint8_t*)buffer, (uint8_t*)buffer + nread ) );
  }
}
