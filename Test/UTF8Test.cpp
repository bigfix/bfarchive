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

#include "BigFix/DataRef.h"
#include "BigFix/UTF8.h"
#include <gtest/gtest.h>

using namespace BigFix;

// These tests are taken from
// http://www.cl.cam.ac.uk/~mgk25/ucs/examples/UTF-8-test.txt

#define GOOD( bytes ) \
  EXPECT_TRUE( IsValidUTF8( reinterpret_cast<const char*>( bytes ) ) );

#define BAD( bytes ) \
  EXPECT_FALSE( IsValidUTF8( reinterpret_cast<const char*>( bytes ) ) );

TEST( UTF8Test, KosmeTest )
{
  // You should see the Greek word 'kosme'.
  uint8_t bytes[] = { 0xCE, 0xBA, 0xE1, 0xBD, 0xB9, 0xCF,
                      0x83, 0xCE, 0xBC, 0xCE, 0xB5, 0x00 };
  GOOD( bytes );
}

TEST( UTF8Test, FirstPossibleSequenceTest )
{
  // 2.1.1  1 byte  (U-00000000)
  {
    uint8_t bytes[] = { 0x00 };
    GOOD( bytes );
  }

  // 2.1.2  2 bytes (U-00000080)
  {
    uint8_t bytes[] = { 0xC2, 0x80, 0x00 };
    GOOD( bytes );
  }

  // 2.1.3  3 bytes (U-00000800)
  {
    uint8_t bytes[] = { 0xE0, 0xA0, 0x80, 0x00 };
    GOOD( bytes );
  }

  // 2.1.4  4 bytes (U-00010000)
  {
    uint8_t bytes[] = { 0xF0, 0x90, 0x80, 0x80, 0x00 };
    GOOD( bytes );
  }

  // 2.1.5  5 bytes (U-00200000)
  {
    uint8_t bytes[] = { 0xF8, 0x88, 0x80, 0x80, 0x80, 0x00 };
    BAD( bytes );
  }

  // 2.1.6  6 bytes (U-04000000)
  {
    uint8_t bytes[] = { 0xFC, 0x84, 0x80, 0x80, 0x80, 0x80, 0x00 };
    BAD( bytes );
  }
}

TEST( UTF8Test, LastPossibleSequenceTest )
{
  // 2.2.1  1 byte  (U-0000007F)
  {
    uint8_t bytes[] = { 0x7f, 0x00 };
    GOOD( bytes );
  }

  // 2.2.2  2 bytes (U-000007FF)
  {
    uint8_t bytes[] = { 0xDF, 0xBF, 0x00 };
    GOOD( bytes );
  }

  // 2.2.3  3 bytes (U-0000FFFF)
  {
    uint8_t bytes[] = { 0xEF, 0xBF, 0xBF, 0x00 };
    GOOD( bytes );
  }

  // 2.2.4  4 bytes (U-001FFFFF)
  {
    uint8_t bytes[] = { 0xF7, 0xBF, 0xBF, 0xBF, 0x00 };
    BAD( bytes );
  }

  // 2.2.5  5 bytes (U-03FFFFFF)
  {
    uint8_t bytes[] = { 0xFB, 0xBF, 0xBF, 0xBF, 0xBF, 0x00 };
    BAD( bytes );
  }

  // 2.2.6  6 bytes (U-7FFFFFFF)
  {
    uint8_t bytes[] = { 0xFD, 0xBF, 0xBF, 0xBF, 0xBF, 0xBF, 0x00 };
    BAD( bytes );
  }
}

TEST( UTF8Test, OtherBoundaryConditionsTest )
{
  // 2.3.1  U-0000D7FF = ed 9f bf
  {
    uint8_t bytes[] = { 0xED, 0x9F, 0xBF, 0x00 };
    GOOD( bytes );
  }

  // 2.3.2  U-0000E000 = ee 80 80
  {
    uint8_t bytes[] = { 0xEE, 0x80, 0x80, 0x00 };
    GOOD( bytes );
  }

  // 2.3.3  U-0000FFFD = ef bf bd
  {
    uint8_t bytes[] = { 0xEF, 0xBF, 0xBD, 0x00 };
    GOOD( bytes );
  }

  // 2.3.4  U-0010FFFF = f4 8f bf bf
  {
    uint8_t bytes[] = { 0xF4, 0x8F, 0xBF, 0xBF, 0x00 };
    GOOD( bytes );
  }

  // 2.3.5  U-00110000 = f4 90 80 80
  {
    uint8_t bytes[] = { 0xF4, 0x90, 0x80, 0x80, 0x00 };
    BAD( bytes );
  }
}

TEST( UTF8Test, UnexpectedContinuationTest )
{
  // 3.1.1  First continuation byte 0x80
  {
    uint8_t bytes[] = { 0x80, 0x00 };
    BAD( bytes );
  }

  // 3.1.2  Last  continuation byte 0xbf
  {
    uint8_t bytes[] = { 0xBF, 0x00 };
    BAD( bytes );
  }

  // 3.1.3  2 continuation bytes
  {
    uint8_t bytes[] = { 0x80, 0xBF, 0x00 };
    BAD( bytes );
  }

  // 3.1.4  3 continuation bytes
  {
    uint8_t bytes[] = { 0x80, 0xBF, 0x80, 0x00 };
    BAD( bytes );
  }

  // 3.1.5  4 continuation bytes
  {
    uint8_t bytes[] = { 0x80, 0xBF, 0x80, 0xBF, 0x00 };
    BAD( bytes );
  }

  // 3.1.6  5 continuation bytes
  {
    uint8_t bytes[] = { 0x80, 0xBF, 0x80, 0xBF, 0x80, 0x00 };
    BAD( bytes );
  }

  // 3.1.7  6 continuation bytes
  {
    uint8_t bytes[] = { 0x80, 0xBF, 0x80, 0xBF, 0x80, 0xBF, 0x00 };
    BAD( bytes );
  }

  // 3.1.8  7 continuation bytes
  {
    uint8_t bytes[] = { 0x80, 0xBF, 0x80, 0xBF, 0x80, 0xBF, 0x80, 0x00 };
    BAD( bytes );
  }
}

TEST( UTF8Test, AllContinuationBytesTest )
{
  // 3.1.9  Sequence of all 64 possible continuation bytes (0x80-0xbf)
  uint8_t bytes[] = { 0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87,
                      0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F,
                      0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97,
                      0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0x9E, 0x9F,
                      0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7,
                      0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF,
                      0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7,
                      0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 0xBE, 0xBF };
  BAD( bytes );
}

TEST( UTF8Test, LonelyStartCharactersTest )
{
  // 3.2.1  All 32 first bytes of 2-byte sequences (0xc0-0xdf),
  // each followed by a space character
  {
    uint8_t bytes[] = { 0xC0, 0x20, 0xC1, 0x20, 0xC2, 0x20, 0xC3, 0x20, 0xC4,
                        0x20, 0xC5, 0x20, 0xC6, 0x20, 0xC7, 0x20, 0xC8, 0x20,
                        0xC9, 0x20, 0xCA, 0x20, 0xCB, 0x20, 0xCC, 0x20, 0xCD,
                        0x20, 0xCE, 0x20, 0xCF, 0x20, 0xD0, 0x20, 0xD1, 0x20,
                        0xD2, 0x20, 0xD3, 0x20, 0xD4, 0x20, 0xD5, 0x20, 0xD6,
                        0x20, 0xD7, 0x20, 0xD8, 0x20, 0xD9, 0x20, 0xDA, 0x20,
                        0xDB, 0x20, 0xDC, 0x20, 0xDD, 0x20, 0xDE, 0x20, 0xDF,
                        0x20, 0x00 };
    BAD( bytes );
  }

  // 3.2.2  All 16 first bytes of 3-byte sequences (0xe0-0xef),
  // each followed by a space character
  {
    uint8_t bytes[] = { 0xE0, 0x20, 0xE1, 0x20, 0xE2, 0x20, 0xE3, 0x20, 0xE4,
                        0x20, 0xE5, 0x20, 0xE6, 0x20, 0xE7, 0x20, 0xE8, 0x20,
                        0xE9, 0x20, 0xEA, 0x20, 0xEB, 0x20, 0xEC, 0x20, 0xED,
                        0x20, 0xEE, 0x20, 0xEF, 0x20, 0x00 };
    BAD( bytes );
  }
  
  // 3.2.3  All 8 first bytes of 4-byte sequences (0xf0-0xf7),
  // each followed by a space character
  {
    uint8_t bytes[] = { 0xF0, 0x20, 0xF1, 0x20, 0xF2, 0x20, 0xF3, 0x20, 0xF4,
                        0x20, 0xF5, 0x20, 0xF6, 0x20, 0xF7, 0x20, 0x00 };
    BAD( bytes );
  }

  // 3.2.4  All 4 first bytes of 5-byte sequences (0xf8-0xfb),
  // each followed by a space character
  {
    uint8_t bytes[] = { 0xF8, 0x20, 0xF9, 0x20, 0xFA, 0x20,
                        0xFB, 0x20, 0x22, 0x20, 0x00 };
    BAD( bytes );
  }
  
  // 3.2.5  All 2 first bytes of 6-byte sequences (0xfc-0xfd),
  // each followed by a space character
  {
    uint8_t bytes[] = { 0xFC, 0x20, 0xFD, 0x20, 0x00 };
    BAD( bytes );
  }
}

TEST( UTF8Test, LastContinuationByteMissingTest )
{
  // 3.3.1  2-byte sequence with last byte missing
  {
    uint8_t bytes[] = { 0xC0, 0x00 };
    BAD( bytes );
  }

  // 3.3.2  3-byte sequence with last byte missing
  {
    uint8_t bytes[] = { 0xE0, 0x80, 0x00 };
    BAD( bytes );
  }

  // 3.3.3  4-byte sequence with last byte missing
  {
    uint8_t bytes[] = { 0xF0, 0x80, 0x80, 0x00 };
    BAD( bytes );
  }

  // 3.3.4  5-byte sequence with last byte missing
  {
    uint8_t bytes[] = { 0xF8, 0x80, 0x80, 0x80, 0x00 };
    BAD( bytes );
  }

  // 3.3.5  6-byte sequence with last byte missing
  {
    uint8_t bytes[] = { 0xFC, 0x80, 0x80, 0x80, 0x00 };
    BAD( bytes );
  }

  // 3.3.6  2-byte sequence with last byte missing
  {
    uint8_t bytes[] = { 0xDF, 0x00 };
    BAD( bytes );
  }

  // 3.3.7  3-byte sequence with last byte missing
  {
    uint8_t bytes[] = { 0xEF, 0xBF, 0x00 };
    BAD( bytes );
  }

  // 3.3.8  4-byte sequence with last byte missing
  {
    uint8_t bytes[] = { 0xF7, 0xBF, 0xBF, 0x00 };
    BAD( bytes );
  }

  // 3.3.9  5-byte sequence with last byte missing
  {
    uint8_t bytes[] = { 0xFB, 0xBF, 0xBF, 0xBF, 0x00 };
    BAD( bytes );
  }

  // 3.3.10 6-byte sequence with last byte missing
  {
    uint8_t bytes[] = { 0xFD, 0xBF, 0xBF, 0xBF, 0xBF, 0x00 };
    BAD( bytes );
  }
}

TEST( UTF8Test, ConcatenationOfIncompleteSequencesTest )
{
  // All the 10 sequences of 3.3 concatenated
  uint8_t bytes[] = { 0xC0, 0xE0, 0x80, 0xF0, 0x80, 0x80, 0xF8, 0x80,
                      0x80, 0x80, 0xFC, 0x80, 0x80, 0x80, 0x80, 0xDF,
                      0xEF, 0xBF, 0xF7, 0xBF, 0xBF, 0xFB, 0xBF, 0xBF,
                      0xBF, 0xFD, 0xBF, 0xBF, 0xBF, 0xBF, 0x00 };
  BAD( bytes );
}

TEST( UTF8Test, ImpossibleBytesTest )
{
  // 3.5.1  fe
  {
    uint8_t bytes[] = { 0xFE, 0x00 };
    BAD( bytes );
  }

  // 3.5.2  ff
  {
    uint8_t bytes[] = { 0xFF, 0x00 };
    BAD( bytes );
  }

  // 3.5.3  fe fe ff ff
  {
    uint8_t bytes[] = { 0xFE, 0xFE, 0xFF, 0xFF, 0x00 };
    BAD( bytes );
  }
}

TEST( UTF8Test, OverlongASCIICharacterTest )
{
  // 4.1.1 U+002F = c0 af
  {
    uint8_t bytes[] = { 0xC0, 0xAF, 0x00 };
    BAD( bytes );
  }

  // 4.1.2 U+002F = e0 80 af
  {
    uint8_t bytes[] = { 0xE0, 0x80, 0xAF, 0x00 };
    BAD( bytes );
  }

  // 4.1.3 U+002F = f0 80 80 af
  {
    uint8_t bytes[] = { 0xF0, 0x80, 0x80, 0xAF, 0x00 };
    BAD( bytes );
  }

  // 4.1.4 U+002F = f8 80 80 80 af
  {
    uint8_t bytes[] = { 0xF8, 0x80, 0x80, 0x80, 0xAF, 0x00 };
    BAD( bytes );
  }

  // 4.1.5 U+002F = fc 80 80 80 80 af
  {
    uint8_t bytes[] = { 0xFC, 0x80, 0x80, 0x80, 0x80, 0xAF, 0x00 };
    BAD( bytes );
  }
}

TEST( UTF8Test, MaximumOverlongSequencesTest )
{
  // 4.2.1  U-0000007F = c1 bf
  {
    uint8_t bytes[] = { 0xC1, 0xBF, 0x00 };
    BAD( bytes );
  }

  // 4.2.2  U-000007FF = e0 9f bf
  {
    uint8_t bytes[] = { 0xE0, 0x9F, 0xBF, 0x00 };
    BAD( bytes );
  }

  // 4.2.3  U-0000FFFF = f0 8f bf bf
  {
    uint8_t bytes[] = { 0xF0, 0x8F, 0xBF, 0xBF, 0x00 };
    BAD( bytes );
  }

  // 4.2.4  U-001FFFFF = f8 87 bf bf bf
  {
    uint8_t bytes[] = { 0xF8, 0x87, 0xBF, 0xBF, 0xBF, 0x00 };
    BAD( bytes );
  }

  // 4.2.5  U-03FFFFFF = fc 83 bf bf bf bf
  {
    uint8_t bytes[] = { 0xFC, 0x83, 0xBF, 0xBF, 0xBF, 0xBF, 0x00 };
    BAD( bytes );
  }
}

TEST( UTF8Test, OverlongRepresentationOfTheNULCharacterTest )
{
  // 4.3.1  U+0000 = c0 80
  {
    uint8_t bytes[] = { 0xC0, 0x80, 0x00 };
    BAD( bytes );
  }

  // 4.3.2  U+0000 = e0 80 80
  {
    uint8_t bytes[] = { 0xE0, 0x80, 0x80, 0x00 };
    BAD( bytes );
  }

  // 4.3.3  U+0000 = f0 80 80 80
  {
    uint8_t bytes[] = { 0xF0, 0x80, 0x80, 0x80, 0x00 };
    BAD( bytes );
  }

  // 4.3.4  U+0000 = f8 80 80 80 80
  {
    uint8_t bytes[] = { 0xF8, 0x80, 0x80, 0x80, 0x80, 0x00 };
    BAD( bytes );
  }

  // 4.3.5  U+0000 = fc 80 80 80 80 80
  {
    uint8_t bytes[] = { 0xFC, 0x80, 0x80, 0x80, 0x80, 0x80, 0x00 };
    BAD( bytes );
  }
}

TEST( UTF8Test, SingleUTF16SurrogatesTest )
{
  // 5.1.1  U+D800 = ed a0 80
  {
    uint8_t bytes[] = { 0xED, 0xA0, 0x80, 0x00 };
    BAD( bytes );
  }

  // 5.1.2  U+DB7F = ed ad bf
  {
    uint8_t bytes[] = { 0xED, 0xAD, 0xBF, 0x00 };
    BAD( bytes );
  }

  // 5.1.3  U+DB80 = ed ae 80
  {
    uint8_t bytes[] = { 0xED, 0xAE, 0x80, 0x00 };
    BAD( bytes );
  }

  // 5.1.4  U+DBFF = ed af bf
  {
    uint8_t bytes[] = { 0xED, 0xAF, 0xBF, 0x00 };
    BAD( bytes );
  }

  // 5.1.5  U+DC00 = ed b0 80
  {
    uint8_t bytes[] = { 0xED, 0xB0, 0x80, 0x00 };
    BAD( bytes );
  }

  // 5.1.6  U+DF80 = ed be 80
  {
    uint8_t bytes[] = { 0xED, 0xBE, 0x80, 0x00 };
    BAD( bytes );
  }

  // 5.1.7  U+DFFF = ed bf bf
  {
    uint8_t bytes[] = { 0xED, 0xBF, 0xBF, 0x00 };
    BAD( bytes );
  }
}

TEST( UTF8Test, PairedUTF16SurrogatesTest )
{
  // 5.2.1  U+D800 U+DC00 = ed a0 80 ed b0 80
  {
    uint8_t bytes[] = { 0xED, 0xA0, 0x80, 0xED, 0xB0, 0x80, 0x00 };
    BAD( bytes );
  }

  // 5.2.2  U+D800 U+DFFF = ed a0 80 ed bf bf
  {
    uint8_t bytes[] = { 0xED, 0xA0, 0x80, 0xED, 0xBF, 0xBF, 0x00 };
    BAD( bytes );
  }

  // 5.2.3  U+DB7F U+DC00 = ed ad bf ed b0 80
  {
    uint8_t bytes[] = { 0xED, 0xAD, 0xBF, 0xED, 0xB0, 0x80, 0x00 };
    BAD( bytes );
  }

  // 5.2.4  U+DB7F U+DFFF = ed ad bf ed bf bf
  {
    uint8_t bytes[] = { 0xED, 0xAD, 0xBF, 0xED, 0xBF, 0xBF, 0x00 };
    BAD( bytes );
  }

  // 5.2.5  U+DB80 U+DC00 = ed ae 80 ed b0 80
  {
    uint8_t bytes[] = { 0xED, 0xAE, 0x80, 0xED, 0xB0, 0x80, 0x00 };
    BAD( bytes );
  }

  // 5.2.6  U+DB80 U+DFFF = ed ae 80 ed bf bf
  {
    uint8_t bytes[] = { 0xED, 0xAE, 0x80, 0xED, 0xBF, 0xBF, 0x00 };
    BAD( bytes );
  }

  // 5.2.7  U+DBFF U+DC00 = ed af bf ed b0 80
  {
    uint8_t bytes[] = { 0xED, 0xAF, 0xBF, 0xED, 0xB0, 0x80, 0x00 };
    BAD( bytes );
  }

  // 5.2.8  U+DBFF U+DFFF = ed af bf ed bf bf
  {
    uint8_t bytes[] = { 0xED, 0xAF, 0xBF, 0xED, 0xBF, 0xBF, 0x00 };
    BAD( bytes );
  }
}
