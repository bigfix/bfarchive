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

#ifndef BigFix_ArgParser_h
#define BigFix_ArgParser_h

#include <string>
#include <vector>

namespace BigFix
{

class ArgParser
{
public:
  void AddOption( char shortName, const std::string& longName );

  void Parse( int argc, const char* argv[] );

  bool HasOption( char shortName ) const;
  bool HasOption( const std::string& longName ) const;

  const std::vector<std::string>& Arguments() const;

private:
  void ParseShortOptions( const char* options );
  void ParseLongOption( const char* option );

  struct Option
  {
    char shortName;
    std::string longName;
  };

  const Option* FindShortOption( char option ) const;
  const Option* FindLongOption( const char* option ) const;
  void SetOption( const Option& );

  std::vector<Option> m_options;

  std::vector<char> m_setShortOptions;
  std::vector<std::string> m_setLongOptions;
  std::vector<std::string> m_arguments;
};

}

#endif
