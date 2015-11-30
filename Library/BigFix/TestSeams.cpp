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

#include "TestSeams.h"
#include <stdio.h>

// Wrap 'deflateInit'.

static Type_deflateInit wrap_deflateInit = Real_deflateInit;

int Real_deflateInit( z_stream* stream, int level )
{
  return deflateInit( stream, level );
}

int Wrap_deflateInit( z_stream* stream, int level )
{
  return wrap_deflateInit( stream, level );
}

void Set_deflateInit( Type_deflateInit wrapFunction )
{
  wrap_deflateInit = wrapFunction;
}

// Wrap 'deflate'.

static Type_deflate wrap_deflate = Real_deflate;

int Real_deflate( z_stream* stream, int flush )
{
  return deflate( stream, flush );
}

int Wrap_deflate( z_stream* stream, int flush )
{
  return wrap_deflate( stream, flush );
}

void Set_deflate( Type_deflate wrapFunction )
{
  wrap_deflate = wrapFunction;
}

// Wrap 'inflateInit'.

static Type_inflateInit wrap_inflateInit = Real_inflateInit;

int Real_inflateInit( z_stream* stream )
{
  return inflateInit( stream );
}

int Wrap_inflateInit( z_stream* stream )
{
  return wrap_inflateInit( stream );
}

void Set_inflateInit( Type_inflateInit wrapFunction )
{
  wrap_inflateInit = wrapFunction;
}

// Wrap 'snprintf'.

static Type_snprintf wrap_snprintf = Real_snprintf;

int Real_snprintf( char* buffer,
                   size_t size,
                   const char* format,
                   const char* dayOfWeek,
                   int day,
                   const char* month,
                   int year,
                   int hour,
                   int minute,
                   int second )
{
#ifdef _WIN32
  return _snprintf_s(
    buffer, size, size, format, dayOfWeek, day, month, year, hour, minute, second );
#else
  return snprintf(
    buffer, size, format, dayOfWeek, day, month, year, hour, minute, second );
#endif
}

int Wrap_snprintf( char* buffer,
                   size_t size,
                   const char* format,
                   const char* dayOfWeek,
                   int day,
                   const char* month,
                   int year,
                   int hour,
                   int minute,
                   int second )
{
  return wrap_snprintf(
    buffer, size, format, dayOfWeek, day, month, year, hour, minute, second );
}

void Set_snprintf( Type_snprintf wrapFunction )
{
  wrap_snprintf = wrapFunction;
}
