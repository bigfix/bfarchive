/*
  Copyright 2014 International Business Machines, Inc.

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
*/

#include "ArchiveCreator.h"
#include "ArchiveExtractor.h"
#include "ArchiveLister.h"
#include "BigFix/ArchiveReader.h"
#include "BigFix/ArgParser.h"
#include "BigFix/DataRef.h"
#include "BigFix/DeflateStream.h"
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
       "  bfarchive -x <source archive> [target directory]\n"
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
  std::cout << BFARCHIVE_VERSION_STRING << std::endl;
  return 0;
}

static void StreamArchive( const std::string& location, ArchiveStream& stream )
{
  ArchiveReader reader( stream );
  InflateStream inflate( reader );

  if ( location == "-" )
    StreamStdIn( inflate );
  else
    StreamFile( location.c_str(), inflate );
}

static int CreateArchive( const std::vector<std::string>& arguments,
                          bool verbose )
{
  if ( arguments.empty() )
  {
    std::cerr << "Must supply a directory or file argument" << std::endl;
    return 1;
  }

  if ( arguments.size() < 2 )
  {
    std::cerr << "Must supply output archive name" << std::endl;
    return 1;
  }

  if ( arguments.size() > 2 )
  {
    std::cerr << "Too many arguments" << std::endl;
    return 1;
  }

  FileStream fileStream;
  fileStream.Reset( OpenNewFile( arguments[1].c_str() ) );

  DeflateStream deflateStream( fileStream );

  ArchiveCreator creator( deflateStream, verbose );
  creator.Create( arguments[0].c_str() );
  return 0;
}

static int ExtractArchive( const std::vector<std::string>& arguments,
                           bool verbose )
{
  if ( arguments.empty() )
  {
    std::cerr << "Must supply an archive to list" << std::endl;
    return 1;
  }

  if ( arguments.size() > 2 )
  {
    std::cerr << "Too many arguments" << std::endl;
    return 1;
  }

  std::string outputDir = ".";

  if ( arguments.size() == 2 )
  {
    MakeDir( arguments[1].c_str() );
    outputDir = arguments[1];
  }

  ArchiveExtractor extractor( outputDir, verbose );
  StreamArchive( arguments[0], extractor );
  return 0;
}

static int ListArchive( const std::vector<std::string>& arguments )
{
  if ( arguments.empty() )
  {
    std::cerr << "Must supply an archive to list" << std::endl;
    return 1;
  }

  if ( arguments.size() > 1 )
  {
    std::cerr << "Too many arguments" << std::endl;
    return 1;
  }

  ArchiveLister lister;
  StreamArchive( arguments[0], lister );
  return 0;
}

int Main( int argc, const char* argv[] )
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
