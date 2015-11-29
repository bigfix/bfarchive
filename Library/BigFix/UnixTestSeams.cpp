/*
  Copyright 2015 International Business Machines, Inc.

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

#include "UnixTestSeams.h"
#include <sys/time.h>

// Wrap 'utimes'.

static Type_utimes wrap_utimes = Real_utimes;

int Real_utimes( const char* filename, const struct timeval* times )
{
  return utimes( filename, times );
}

int Wrap_utimes( const char* filename, const struct timeval* times )
{
  return wrap_utimes( filename, times );
}

void Set_utimes( Type_utimes wrapFunction )
{
  wrap_utimes = wrapFunction;
}
