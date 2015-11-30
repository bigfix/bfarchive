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
#include <unistd.h>

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

// Wrap 'read'.

static Type_read wrap_read = Real_read;

int Real_read( int fd, void* buf, size_t count )
{
  return read( fd, buf, count );
}

int Wrap_read( int fd, void* buf, size_t count )
{
  return wrap_read( fd, buf, count );
}

void Set_read( Type_read wrapFunction )
{
  wrap_read = wrapFunction;
}

// Wrap 'write'.

static Type_write wrap_write = Real_write;

int Real_write( int fd, const void* buf, size_t count )
{
  return write( fd, buf, count );
}

int Wrap_write( int fd, const void* buf, size_t count )
{
  return wrap_write( fd, buf, count );
}

void Set_write( Type_write wrapFunction )
{
  wrap_write = wrapFunction;
}
