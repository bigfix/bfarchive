#include "Error.h"

namespace BigFix
{

const char* Error::what() const throw()
{
  return m_what;
}

}
