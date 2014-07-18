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

#ifndef BigFix_Number_h
#define BigFix_Number_h

#include <stddef.h>
#include <stdint.h>

namespace BigFix
{

class DataRef;

uint64_t ReadLittleEndian( DataRef );
void WriteLittleEndian( uint64_t, uint8_t* buffer, size_t length );

template <class T>
T ReadAsciiNumber( DataRef );

}

#endif
