#include "ArchiveExtractor.h"
#include "BigFix/ArchiveReader.h"
#include "BigFix/DataRef.h"
#include "BigFix/Filesystem.h"
#include "BigFix/InflateStream.h"
#include <iostream>

using namespace BigFix;

static void PrintUsage()
{
  std::cout << "Usage: bfarchive -x <source-archive> <output-directory>\n"
               "\n"
               "Where:\n"
               "  -x, --extract Extract an archive\n"
               "  -h, --help    Print this help\n"
               "  -v, --version Print the version and exit\n";
}

int main( int argc, const char* argv[] )
{
  if ( argc != 4 )
  {
    PrintUsage();
    return 1;
  }

  try
  {
    MakeDir( argv[3], ENCODING_UTF8 );

    std::auto_ptr<File> archive = OpenExistingFile( argv[2], ENCODING_UTF8 );

    ArchiveExtractor extractor;
    ArchiveReader reader( extractor );
    InflateStream inflator( reader );

    uint8_t buffer[4096];

    while ( true )
    {
      size_t nread = archive->Read( buffer, sizeof( buffer ) );

      if ( nread == 0 )
        break;
      
      inflator.Write( DataRef( buffer, buffer + nread ) );
    }

    inflator.End();
  }
  catch ( const std::exception& err )
  {
    std::cerr << "Error extracting archive: " << err.what() << std::endl;
    return 1;
  }

  return 0;
}
