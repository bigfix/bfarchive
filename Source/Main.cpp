#include "ArchiveExtractor.h"
#include "BigFix/ArchiveReader.h"
#include "BigFix/DataRef.h"
#include "BigFix/Filesystem.h"
#include "BigFix/InflateStream.h"
#include <iostream>

using namespace BigFix;

static void PrintUsage()
{
  std::cout
    << "Usage:\n"
       "  bfarchive -a <source file or directory> <name of archive>\n"
       "  bfarchive -x <source archive> <target directory>\n"
       "  bfarchive -l <source archive>\n"
       "\n"
       "Where:\n"
       "  -a, --archive    Create an archive\n"
       "  -h, --help       Print this help message\n"
       "  -l, --list       List the archive contents\n"
       "  -v, --verbose    Print every file and directory as it is processed\n"
       "  -V, --version    Print the version and exit\n"
       "  -x, --extract    Extract an archive\n";
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
    MakeDir( argv[3] );
    std::auto_ptr<File> archive = OpenExistingFile( argv[2] );

    ArchiveExtractor extractor( argv[3] );
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
