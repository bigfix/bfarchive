#include "ArgParser.h"
#include "BigFix/Error.h"

namespace BigFix
{

void ArgParser::AddOption( char shortName, const std::string& longName )
{
  Option option = { shortName, longName };
  m_options.push_back( option );
}

void ArgParser::Parse( int argc, const char* argv[] )
{
  for ( int i = 1; i < argc; i++ )
  {
    if ( argv[i][0] == '-' && argv[i][1] == '-' )
      ParseLongOption( argv[i] + 2 );
    else if ( argv[i][0] == '-' && argv[i][1] )
      ParseShortOptions( argv[i] + 1 );
    else
      m_arguments.push_back( argv[i] );
  }
}

bool ArgParser::HasOption( char shortName ) const
{
  return std::find( m_setShortOptions.begin(),
                    m_setShortOptions.end(),
                    shortName ) != m_setShortOptions.end();
}

bool ArgParser::HasOption( const std::string& longName ) const
{
  return std::find( m_setLongOptions.begin(),
                    m_setLongOptions.end(),
                    longName ) != m_setLongOptions.end();
}

const std::vector<std::string>& ArgParser::Arguments() const
{
  return m_arguments;
}

void ArgParser::ParseShortOptions( const char* options )
{
  for ( const char* option = options; *option; option++ )
  {
    const Option* matchingOption = FindShortOption( *option );

    if ( !matchingOption )
      throw Error( "Unknown option value" );

    SetOption( *matchingOption );
  }
}

void ArgParser::ParseLongOption( const char* option )
{
  const Option* matchingOption = FindLongOption( option );

  if ( !matchingOption )
    throw Error( "Unknown option value" );

  SetOption( *matchingOption );
}

const ArgParser::Option* ArgParser::FindShortOption( char option ) const
{
  for ( std::vector<Option>::const_iterator it = m_options.begin(),
                                            end = m_options.end();
        it != end;
        it++ )
  {
    if ( it->shortName == option )
      return &*it;
  }

  return 0;
}

const ArgParser::Option* ArgParser::FindLongOption( const char* option ) const
{
  for ( std::vector<Option>::const_iterator it = m_options.begin(),
                                            end = m_options.end();
        it != end;
        it++ )
  {
    if ( it->longName == option )
      return &*it;
  }

  return 0;
}

void ArgParser::SetOption( const Option& option )
{
  m_setShortOptions.push_back( option.shortName );
  m_setLongOptions.push_back( option.longName );
}

}
