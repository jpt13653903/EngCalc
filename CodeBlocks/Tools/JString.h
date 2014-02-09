//==============================================================================
// Copyright (C) John-Philip Taylor
// jpt13653903@gmail.com
//
// This file is part of a library
//
// This file is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>
//==============================================================================

#ifndef JString_h
#define JString_h
//------------------------------------------------------------------------------

#include <math.h>
//------------------------------------------------------------------------------

#include "UnicodeCodec.h"
//------------------------------------------------------------------------------

class STRING{
 private:
  unsigned StringLength;
  unsigned Allocated;

  void Expand(unsigned MinLength);

 public:
  char* String;

   STRING();
  ~STRING();

  void Clear   ();
  void Reverse ();
  void Allocate(unsigned Size); // Also clears the string

  int  Length   ();
  void SetLength(int Length); // Moves the null-terminator and updates the
                              // Length (must be less than or equal to
                              // the current length)

  void Set    (STRING*        String);
  void Set    (const char*    String);
  void Set    (const wchar_t* String);
  void Set    (char     c);
  void Set    (char32_t c);
  void Set    (int      i);
  void Set    (unsigned i, int Places); // Hexadecimal
  void Set    (double   d, int SignificantFigures = 9, bool Fill = false);
  void Append (STRING*        String);
  void Append (const char*    String);
  void Append (const wchar_t* String);
  void Append (char     c);
  void Append (char32_t c);
  void Append (int      i);
  void Append (unsigned i, int Places); // Hexadecimal
  void Append (double   d, int SignificantFigures = 9, bool Fill = false);
  int  Compare(STRING*     String);
  int  Compare(const char* String);

  int  CompareNoCase(STRING*     String);
  int  CompareNoCase(const char* String);
};
//------------------------------------------------------------------------------

#endif
//------------------------------------------------------------------------------
