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
