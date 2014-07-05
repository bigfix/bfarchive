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
       "  -a, --create     Create an archive\n"
       "  -x, --extract    Extract an archive\n"
       "  -h, --help       Print this help message\n"
       "  -l, --list       List the archive contents\n"
       "  -v, --verbose    Print every file and directory as it is processed\n"
       "  -V, --version    Print the version and exit\n";
}

int main( int argc, const char* argv[] )
{
  try
  {
    ArgParser argParser;
    argParser.AddOption( 'a', "create" );
    argParser.AddOption( 'h', "help" );
    argParser.AddOption( 'l', "list" );
    argParser.AddOption( 'v', "verbose" );
    argParser.AddOption( 'V', "version" );
    argParser.AddOption( 'x', "extract" );
    argParser.Parse( argc, argv );

    if ( argParser.HasOption( "help" ) )
    {
      PrintUsage();
    }
    else if ( argParser.HasOption( "version" ) )
    {
      std::cout << "Print version\n";
    }
    else if ( argParser.HasOption( "create" ) )
    {
      std::cout << "Create archive\n";

      if ( argParser.HasOption( "verbose" ) )
        std::cout << "Verbose\n";
    }
    else if ( argParser.HasOption( "extract" ) )
    {
      std::cout << "Extract archive\n";

      if ( argParser.HasOption( "verbose" ) )
        std::cout << "Verbose\n";
    }
    else if ( argParser.HasOption( "list" ) )
    {
      std::cout << "List archive\n";
    }
    else
    {
      PrintUsage();
      return 1;
    }
  }
  catch ( const std::exception& err )
  {
    std::cerr << "Error: " << err.what() << std::endl;
    return 1;
  }

  return 0;
}
