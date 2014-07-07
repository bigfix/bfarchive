#include "Main.h"
#include "BigFix/WindowsFilesystem.h"
#include <vector>

using namespace BigFix;

int wmain( int argc, const wchar_t* argv[] )
{
  std::vector<std::string> utf8Args;
  std::vector<const char*> utf8Argv;

  for ( int i = 0; i < argc; i++ )
    utf8Args.push_back( MakePortableFilePath( argv[i] ) );

  for ( int i = 0; i < argc; i++ )
    utf8Argv.push_back( utf8Args[i].c_str() );

  return Main( argc, utf8Argv.empty() ? NULL : &utf8Argv.front() );
}
