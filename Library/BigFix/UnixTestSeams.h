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

#ifndef BigFix_UnixTestSeams_h
#define BigFix_UnixTestSeams_h

#include <stddef.h>
#include <dirent.h>

// Wrap 'utimes'.

int Real_utimes( const char*, const struct timeval* );
int Wrap_utimes( const char*, const struct timeval* );

typedef int ( *Type_utimes )( const char*, const struct timeval* );
void Set_utimes( Type_utimes );

// Wrap 'read'.

int Real_read( int, void*, size_t );
int Wrap_read( int, void*, size_t );

typedef int ( *Type_read )( int, void*, size_t );
void Set_read( Type_read );

// Wrap 'write'.

int Real_write( int, const void*, size_t );
int Wrap_write( int, const void*, size_t );

typedef int ( *Type_write )( int, const void*, size_t );
void Set_write( Type_write );

// Wrap 'readdir_r'.

int Real_readdir_r( DIR*, struct dirent*, struct dirent** );
int Wrap_readdir_r( DIR*, struct dirent*, struct dirent** );

typedef int ( *Type_readdir_r )( DIR*, struct dirent*, struct dirent** );
void Set_readdir_r( Type_readdir_r );

#endif
