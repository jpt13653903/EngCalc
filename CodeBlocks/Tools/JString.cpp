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

#include "JString.h"
//------------------------------------------------------------------------------

STRING::STRING(){
 StringLength = 0;
 Allocated    = 4;
 String       = new char[Allocated];
 String[0]    = 0;
}
//------------------------------------------------------------------------------

STRING::~STRING(){
 delete[] String;
}
//------------------------------------------------------------------------------

void STRING::Expand(unsigned MinLength){
 if(MinLength >= Allocated){
  while(MinLength >= Allocated) Allocated <<= 1;

  unsigned j;
  char*    Temp = new char[Allocated];

  for(j = 0; j < StringLength; j++) Temp[j] = String[j];
  Temp[j] = 0;

  delete[] String;
  String = Temp;
 }
}
//------------------------------------------------------------------------------

void STRING::Clear(){
 Allocate(0);
}
//------------------------------------------------------------------------------

void STRING::Allocate(unsigned Size){
 Allocated = 4;
 while(Allocated < Size+1) Allocated <<= 1;

 delete[] String;
 StringLength = 0;
 String       = new char[Allocated];
 String[0]    = 0;
}
//------------------------------------------------------------------------------

int STRING::Length(){
 return StringLength;
}
//------------------------------------------------------------------------------

void STRING::SetLength(int Length){
 if(Length >= 0 && Length < (int)StringLength){
  StringLength   = Length;
  String[Length] = 0;
 }
}
//------------------------------------------------------------------------------

void STRING::Set(STRING* String){
 unsigned j = String->StringLength;

 Allocate(j);
 for(j = 0; j < String->StringLength; j++){
  STRING::String[j] = String->String[j];
 }
 STRING::String[j] = 0;
 StringLength = j;
}
//------------------------------------------------------------------------------

void STRING::Set(const char* String){
 int j;
 for(j = 0; String[j]; j++);

 Allocate(j);
 for(j = 0; String[j]; j++) STRING::String[j] = String[j];
 STRING::String[j] = 0;
 StringLength = j;
}
//------------------------------------------------------------------------------

void STRING::Set(const wchar_t* String){
 UNICODE_CODEC Codec;

 char*    UTF8 = Codec.GetUTF8(String);
 Set     (UTF8);
 delete[] UTF8;
}
//------------------------------------------------------------------------------

void STRING::Set(char c){
 Allocate(1);
 String[StringLength++] = c;
 String[StringLength  ] = 0;
}
//------------------------------------------------------------------------------

void STRING::Set(char32_t c){
 UNICODE_CODEC Codec;

 char32_t Unicode[2] = {c, 0};
 char*    UTF8       = Codec.GetUTF8(Unicode);
 Set     (UTF8);
 delete[] UTF8;
}
//------------------------------------------------------------------------------

void STRING::Set(int i){
 bool Negative = false;

 if(i){
  Clear();

  if(i < 0){
   i *= -1;
   Negative = true;
  }

  while(i){ // To append is faster that to prefix
   Append((char)((i % 10) + '0'));
   i /= 10;
  }

  if(Negative) Append('-');

  Reverse();

 }else{
  Set("0");
 }
}
//------------------------------------------------------------------------------

void STRING::Set(unsigned i, int Places){ // Hexadecimal
 bool Negative = false;
 char c;

 Clear();

 while(i){ // To append is faster that to prefix
  c = (char)(i & 0x0F);
  if(c <= 9){
   c += '0';
  }else{
   c += 'A' - 10;
  }
  Append(c);
  Places--;
  i >>= 4;
 }

 while(Places > 0){
  Append("0");
  Places--;
 }

 if(Negative) Append('-');

 Reverse();
}
//------------------------------------------------------------------------------

void STRING::Set(double d, int SignificantFigures, bool Fill){
 int    j, shift, places;
 double f;
 STRING s;

 unsigned* u1 = (unsigned*)&f;
 unsigned* u2 = (unsigned*)&d;

 f = 1.0/0.0;
 if(u1[0] == u2[0] && u1[1] == u2[1]){
  Set("INF");
  return;
 }
 f = -1.0/0.0;
 if(u1[0] == u2[0] && u1[1] == u2[1]){
  Set("-INF");
  return;
 }
 f = 0.0/0.0;
 if(u1[0] == u2[0] && u1[1] == u2[1]){
  Set("NAN");
  return;
 }

 if(d < 0.0){
  Set('-');
  d *= -1.0;
 }else{
  Clear();
 }

 if(d == 0.0){
  Set("0");
  return;
 }

 // Find the shift to get the first significant figure at the unit position
 f     = d;
 shift = 0;
 while(f >= 10.0){
  f /= 10.0;
  shift++;
 }
 while(f < 1.0){
  f *= 10.0;
  shift--;
 }

 // Round accordingly and store as integer
 f = pow  (10.0, SignificantFigures - shift - 1);
 d = round(d * f);

 // Convert the integer to characters
 places = 0;
 while(d >= 1.0){
  s.Append((char)(fmod(d, 10.0) + '0'));
  d /= 10.0;
  places++;
 }
 s.Reverse();

 // Correct for a round up
 if(places > SignificantFigures) shift++;

 // Convert to correct output format
 if(shift < 0){
  Append("0.");
  for(j = 1; j < -shift; j++) Append('0');
  for(j = 0; j < places; j++) Append(s.String[j]);
  if(!Fill){
   while(String[StringLength-1] == '0') StringLength--;
   if   (String[StringLength-1] == '.') StringLength--;
   String[StringLength] = 0;
  }

 }else{
  for(j = 0; j < places && j < shift+1; j++) Append(s.String[j]);
  if(j < places){
   Append('.');
   for(; j < places ; j++) Append(s.String[j]);
   if(!Fill){
    while(String[StringLength-1] == '0') StringLength--;
    if   (String[StringLength-1] == '.') StringLength--;
    String[StringLength] = 0;
   }
  }else{
   for(; j < shift+1; j++) Append('0');
  }
 }
}
//------------------------------------------------------------------------------

void STRING::Append(STRING* String){
 unsigned j = String->StringLength;

 Expand(StringLength + j);

 for(j = 0; j < String->StringLength; j++){
  STRING::String[StringLength++] = String->String[j];
 }
 STRING::String[StringLength] = 0;
}
//------------------------------------------------------------------------------

void STRING::Append(const char* String){
 unsigned j;
 for(j = 0; String[j]; j++);

 Expand(StringLength + j);

 for(j = 0; String[j]; j++) STRING::String[StringLength++] = String[j];
 STRING::String[StringLength] = 0;
}
//------------------------------------------------------------------------------

void STRING::Append(const wchar_t* String){
 UNICODE_CODEC Codec;

 char*    UTF8 = Codec.GetUTF8(String);
 Append  (UTF8);
 delete[] UTF8;
}
//------------------------------------------------------------------------------

void STRING::Append(char c){
 Expand(StringLength + 1);

 String[StringLength++] = c;
 String[StringLength  ] = 0;
}
//------------------------------------------------------------------------------

void STRING::Append(char32_t c){
 UNICODE_CODEC Codec;

 char32_t Unicode[2] = {c, 0};
 char*    UTF8       = Codec.GetUTF8(Unicode);
 Append  (UTF8);
 delete[] UTF8;
}
//------------------------------------------------------------------------------

void STRING::Append(int i){
 STRING s;
 s.Set(i);
 Append(s.String);
}
//------------------------------------------------------------------------------

void STRING::Append(unsigned i, int Places){
 STRING s;
 s.Set(i, Places);
 Append(s.String);
}
//------------------------------------------------------------------------------

void STRING::Append(double d, int SignificantFigures, bool Fill){
 STRING s;
 s.Set(d, SignificantFigures, Fill);
 Append(s.String);
}
//------------------------------------------------------------------------------

// Returns 0 when strings are equal
int STRING::Compare(STRING* String){
 return Compare(String->String);
}
//------------------------------------------------------------------------------

// Returns 0 when strings are equal
int STRING::Compare(const char* String){
 int j;

 for(j = 0; STRING::String[j] && String[j]; j++){
  if(STRING::String[j] < String[j]) return -1;
  if(STRING::String[j] > String[j]) return  1;
 }
 if(STRING::String[j] < String[j]) return -1;
 if(STRING::String[j] > String[j]) return  1;

 return 0;
}
//------------------------------------------------------------------------------

char UpperCase(char c){
 if(c >= 'a' && c <= 'z') return c - 'a' + 'A';
 return c;
}
//------------------------------------------------------------------------------

int STRING::CompareNoCase(STRING* String){
 return CompareNoCase(String->String);
}
//------------------------------------------------------------------------------

int STRING::CompareNoCase(const char* String){
 int j;

 for(j = 0; STRING::String[j] && String[j]; j++){
  if(UpperCase(STRING::String[j]) < UpperCase(String[j])) return -1;
  if(UpperCase(STRING::String[j]) > UpperCase(String[j])) return  1;
 }
 if(UpperCase(STRING::String[j]) < UpperCase(String[j])) return -1;
 if(UpperCase(STRING::String[j]) > UpperCase(String[j])) return  1;

 return 0;
}
//------------------------------------------------------------------------------

void STRING::Reverse(){
 int  j, l;
 char c;

 l = StringLength/2;

 for(j = 0; j < l; j++){
  c = String[j];
  String[j] = String[StringLength-j-1];
  String[StringLength-j-1] = c;
 }
}
//------------------------------------------------------------------------------
