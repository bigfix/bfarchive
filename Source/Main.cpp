#include "ArchiveExtractor.h"
#include "BigFix/ArchiveReader.h"
#include "BigFix/ArgParser.h"
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
  try
  {
    ArgParser argParser;
    argParser.AddOption( 'a', "archive" );
    argParser.AddOption( 'h', "help" );
    argParser.AddOption( 'l', "list" );
    argParser.AddOption( 'v', "verbose" );
    argParser.AddOption( 'V', "version" );
    argParser.AddOption( 'x', "extract" );
    argParser.Parse( argc, argv );

    if ( argParser.HasOption( 'h' ) )
    {
      PrintUsage();
    }
    else if ( argParser.HasOption( 'V' ) )
    {
      std::cout << "Print version\n";
    }
    else if ( argParser.HasOption( 'a' ) )
    {
      std::cout << "Create archive\n";

      if ( argParser.HasOption( 'v' ) )
        std::cout << "Verbose\n";
    }
    else if ( argParser.HasOption( 'x' ) )
    {
      std::cout << "Extract archive\n";

      if ( argParser.HasOption( 'v' ) )
        std::cout << "Verbose\n";
    }
    else if ( argParser.HasOption( 'l' ) )
    {
      std::cout << "List archive\n";
    }
    else
    {
      PrintUsage();
    }
  }
  catch ( const std::exception& err )
  {
    std::cerr << "Error: " << err.what() << std::endl;
    return 1;
  }

  return 0;
}
