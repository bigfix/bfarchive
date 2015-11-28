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

#ifndef BigFix_TestSeams_h
#define BigFix_TestSeams_h

#include <zlib.h>

// Wrap 'deflateInit'.

int Real_deflateInit( z_stream*, int );
int Wrap_deflateInit( z_stream*, int );

typedef int ( *Type_deflateInit )( z_stream*, int );
void Set_deflateInit( Type_deflateInit );

// Wrap 'deflate'.

int Real_deflate( z_stream*, int );
int Wrap_deflate( z_stream*, int );

typedef int ( *Type_deflate )( z_stream*, int );
void Set_deflate( Type_deflate );

// Wrap 'inflateInit'.

int Real_inflateInit( z_stream* );
int Wrap_inflateInit( z_stream* );

typedef int ( *Type_inflateInit )( z_stream* );
void Set_inflateInit( Type_inflateInit );

// Wrap 'snprintf'.

int Real_snprintf( z_stream* );
int Wrap_snprintf( z_stream* );

typedef int ( *Type_snprintf )( z_stream* );
void Set_snprintf( Type_snprintf );

#endif
