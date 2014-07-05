#include "ArchiveExtractor.h"
#include "ArchiveLister.h"
#include "BigFix/ArchiveReader.h"
#include "BigFix/ArgParser.h"
#include "BigFix/DataRef.h"
#include "BigFix/Filesystem.h"
#include "BigFix/InflateStream.h"
#include "Version.h"
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

static int PrintVersion()
{
  std::cout << BFARCHIVE_VERSION_STRING << "\n";
  return 0;
}

static int CreateArchive( const std::vector<std::string>& arguments,
                          bool verbose )
{
  std::cout << "CreateArchive" << std::endl;
  return 0;
}

static int ExtractArchive( const std::vector<std::string>& arguments,
                           bool verbose )
{
  std::cout << "ExtractArchive" << std::endl;
  return 0;
}

static int ListArchive( const std::vector<std::string>& arguments )
{
  if ( arguments.empty() )
  {
    std::cerr << "Must supply an archive to list\n";
    return 1;
  }

  if ( arguments.size() > 1 )
  {
    std::cerr << "Too many arguments\n";
    return 1;
  }

  ArchiveLister lister;
  ArchiveReader reader( lister );
  InflateStream inflate( reader );

  if ( arguments[0] == "-" )
    ReadStdIn( inflate );
  else
    ReadFile( arguments[0].c_str(), inflate );

  return 0;
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
      return 0;
    }

    if ( argParser.HasOption( "version" ) )
      return PrintVersion();

    if ( argParser.HasOption( "create" ) )
    {
      return CreateArchive( argParser.Arguments(),
                            argParser.HasOption( "verbose" ) );
    }

    if ( argParser.HasOption( "extract" ) )
    {
      return ExtractArchive( argParser.Arguments(),
                             argParser.HasOption( "verbose" ) );
    }

    if ( argParser.HasOption( "list" ) )
      return ListArchive( argParser.Arguments() );

    PrintUsage();
    return 1;
  }
  catch ( const std::exception& err )
  {
    std::cerr << "Error: " << err.what() << std::endl;
    return 1;
  }

  return 0;
}
