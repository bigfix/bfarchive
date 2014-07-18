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

#ifndef BigFix_Error_h
#define BigFix_Error_h

#include <exception>
#include <stddef.h>

namespace BigFix
{

class Error : public std::exception
{
public:
  template <size_t n>
  explicit Error( const char ( &literal )[n] ) throw()
    : m_what( literal )
  {
  }

  virtual const char* what() const throw();

private:
  const char* m_what;
};

}

#endif
