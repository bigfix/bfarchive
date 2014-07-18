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
