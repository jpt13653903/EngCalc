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

#include "UnicodeCodec.h"
//------------------------------------------------------------------------------

char* UNICODE_CODEC::GetUTF8(const wchar_t* Text){
 char16_t* UTF16 = GetUTF16(Text);
 char    * UTF8  = GetUTF8 (UTF16);
 delete[]  UTF16;
 return    UTF8;
}
//------------------------------------------------------------------------------

char* UNICODE_CODEC::GetUTF8(const char16_t* Text){
 char32_t* Unicode = GetUnicode(Text);
 char    * UTF8    = GetUTF8   (Unicode);
 delete[]  Unicode;
 return    UTF8;
}
//------------------------------------------------------------------------------

char* UNICODE_CODEC::GetUTF8(const char32_t* Text){
 int t, u;
 for(t = 0; Text[t]; t++);

 unsigned char* UTF8 = new unsigned char[t*6 + 1];

 t = 0;
 u = 0;
 while(Text[t]){
  if(Text[t] >> 31){ // Invalid code-point for UTF-8, so do nothing

  }else if(Text[t] >> 26){ // 6-Byte
   UTF8[u++] = 0xFC | ((Text[t] >> 30)       );
   UTF8[u++] = 0x80 | ((Text[t] >> 24) & 0x3F);
   UTF8[u++] = 0x80 | ((Text[t] >> 18) & 0x3F);
   UTF8[u++] = 0x80 | ((Text[t] >> 12) & 0x3F);
   UTF8[u++] = 0x80 | ((Text[t] >>  6) & 0x3F);
   UTF8[u++] = 0x80 | ((Text[t]      ) & 0x3F);

  }else if(Text[t] >> 21){ // 5-Byte
   UTF8[u++] = 0xF8 | ((Text[t] >> 24)       );
   UTF8[u++] = 0x80 | ((Text[t] >> 18) & 0x3F);
   UTF8[u++] = 0x80 | ((Text[t] >> 12) & 0x3F);
   UTF8[u++] = 0x80 | ((Text[t] >>  6) & 0x3F);
   UTF8[u++] = 0x80 | ((Text[t]      ) & 0x3F);

  }else if(Text[t] >> 16){ // 4-Byte
   UTF8[u++] = 0xF0 | ((Text[t] >> 18)       );
   UTF8[u++] = 0x80 | ((Text[t] >> 12) & 0x3F);
   UTF8[u++] = 0x80 | ((Text[t] >>  6) & 0x3F);
   UTF8[u++] = 0x80 | ((Text[t]      ) & 0x3F);

  }else if(Text[t] >> 11){ // 3-Byte
   UTF8[u++] = 0xE0 | ((Text[t] >> 12)       );
   UTF8[u++] = 0x80 | ((Text[t] >>  6) & 0x3F);
   UTF8[u++] = 0x80 | ((Text[t]      ) & 0x3F);

  }else if(Text[t] >>  7){ // 2-Byte
   UTF8[u++] = 0xC0 | ((Text[t] >>  6)       );
   UTF8[u++] = 0x80 | ((Text[t]      ) & 0x3F);

  }else{ // 1-Byte
   UTF8[u++] = Text[t];
  }
  t++;
 }
 UTF8[u] = 0;

 return (char*)UTF8;
}
//------------------------------------------------------------------------------

char16_t* UNICODE_CODEC::GetUTF16(const char* Text){
 char32_t* Unicode = GetUnicode(Text);
 char16_t* UTF16   = GetUTF16  (Unicode);
 delete[]  Unicode;
 return    UTF16;
}
//------------------------------------------------------------------------------

char16_t* UNICODE_CODEC::GetUTF16(const unsigned char* Text){
 char32_t* Unicode = GetUnicode(Text);
 char16_t* UTF16   = GetUTF16  (Unicode);
 delete[]  Unicode;
 return    UTF16;
}
//------------------------------------------------------------------------------

char16_t* UNICODE_CODEC::GetUTF16(const wchar_t* Text){
 int j;
 for(j = 0; Text[j]; j++);

 char16_t* UTF16 = new char16_t[j+1];

 for(j = 0; Text[j]; j++) UTF16[j] = Text[j];
 UTF16[j] = 0;

 return UTF16;
}
//------------------------------------------------------------------------------

char16_t* UNICODE_CODEC::GetUTF16(const char32_t* Text){
 int t, u;
 for(t = 0; Text[t]; t++);

 char16_t* UTF16 = new char16_t[t*2 + 1];

 t = 0;
 u = 0;
 while(Text[t]){
  if(Text[t] >> 16){ // Encoded
   UTF16[u++] = ((Text[t] & 0x000FFFFF) >> 10) | 0xD800;
   UTF16[u++] = ((Text[t] & 0x000003FF)      ) | 0xDC00;

  }else{ // Direct
   UTF16[u++] = Text[t];
  }
  t++;
 }
 UTF16[u] = 0;

 return UTF16;
}
//------------------------------------------------------------------------------

char32_t* UNICODE_CODEC::GetUnicode(const char* Text){
 return GetUnicode((unsigned char*)Text);
}
//------------------------------------------------------------------------------

char32_t* UNICODE_CODEC::GetUnicode(const unsigned char* Text){
 unsigned t, u;

 for(t = 0; Text[t]; t++);
 char32_t* Unicode = new char32_t[t+1];

 t = 0;
 u = 0;
 while(Text[t]){
  if(Text[t] & 0x80){
   if(Text[t] & 0x40){
    if(Text[t] & 0x20){
     if(Text[t] & 0x10){
      if(Text[t] & 0x08){
       if(Text[t] & 0x04){
        if(Text[t] & 0x02){ // Invalid byte
         Unicode[u] = Text[t++];
        }else{ // 6 bytes
         Unicode[u] = Text[t++] & 0x01;
         if((Text[t] & 0xC0) == 0x80)
          Unicode[u] = (Unicode[u] << 6) | (Text[t++] & 0x3F);
         if((Text[t] & 0xC0) == 0x80)
          Unicode[u] = (Unicode[u] << 6) | (Text[t++] & 0x3F);
         if((Text[t] & 0xC0) == 0x80)
          Unicode[u] = (Unicode[u] << 6) | (Text[t++] & 0x3F);
         if((Text[t] & 0xC0) == 0x80)
          Unicode[u] = (Unicode[u] << 6) | (Text[t++] & 0x3F);
         if((Text[t] & 0xC0) == 0x80)
          Unicode[u] = (Unicode[u] << 6) | (Text[t++] & 0x3F);
        }
       }else{ // 5 bytes
        Unicode[u] = Text[t++] & 0x03;
        if((Text[t] & 0xC0) == 0x80)
         Unicode[u] = (Unicode[u] << 6) | (Text[t++] & 0x3F);
        if((Text[t] & 0xC0) == 0x80)
         Unicode[u] = (Unicode[u] << 6) | (Text[t++] & 0x3F);
        if((Text[t] & 0xC0) == 0x80)
         Unicode[u] = (Unicode[u] << 6) | (Text[t++] & 0x3F);
        if((Text[t] & 0xC0) == 0x80)
         Unicode[u] = (Unicode[u] << 6) | (Text[t++] & 0x3F);
       }
      }else{ // 4 bytes
       Unicode[u] = Text[t++] & 0x07;
       if((Text[t] & 0xC0) == 0x80)
        Unicode[u] = (Unicode[u] << 6) | (Text[t++] & 0x3F);
       if((Text[t] & 0xC0) == 0x80)
        Unicode[u] = (Unicode[u] << 6) | (Text[t++] & 0x3F);
       if((Text[t] & 0xC0) == 0x80)
        Unicode[u] = (Unicode[u] << 6) | (Text[t++] & 0x3F);
      }
     }else{ // 3 bytes
      Unicode[u] = Text[t++] & 0x0F;
      if((Text[t] & 0xC0) == 0x80)
       Unicode[u] = (Unicode[u] << 6) | (Text[t++] & 0x3F);
      if((Text[t] & 0xC0) == 0x80)
       Unicode[u] = (Unicode[u] << 6) | (Text[t++] & 0x3F);
     }
    }else{ // 2 bytes
     Unicode[u] = Text[t++] & 0x1F;
     if((Text[t] & 0xC0) == 0x80)
      Unicode[u] = (Unicode[u] << 6) | (Text[t++] & 0x3F);
    }
   }else{ // Unsynchronised continuation byte
    Unicode[u] = Text[t++];
   }
  }else{ // 1 byte
   Unicode[u] = Text[t++];
  }
  u++;
 }
 Unicode[u] = 0;

 return Unicode;
}
//------------------------------------------------------------------------------

char32_t* UNICODE_CODEC::GetUnicode(const wchar_t* Text){
 char16_t* UTF16   = GetUTF16  (Text);
 char32_t* Unicode = GetUnicode(UTF16);
 delete[]  UTF16;
 return    Unicode;
}
//------------------------------------------------------------------------------

char32_t* UNICODE_CODEC::GetUnicode(const char16_t* Text){
 unsigned t, u;

 for(t = 0; Text[t]; t++);
 char32_t* Unicode = new char32_t[t+1];

 t = 0;
 u = 0;
 while(Text[t]){
  if(
   ((Text[t  ] & 0xFC00) == 0xD800) &&
   ((Text[t+1] & 0xFC00) == 0xDC00)
  ){ // 2 words
   Unicode[u] =                       Text[t++] & 0x03FF;
   Unicode[u] = (Unicode[u] << 10) | (Text[t++] & 0x03FF);
  }else{ // 1 word
   Unicode[u] = Text[t++];
  }
  u++;
 }
 Unicode[u] = 0;

 return Unicode;
}
//------------------------------------------------------------------------------

wchar_t* UNICODE_CODEC::GetWideString(const char* Text){
 char16_t* UTF16      = GetUTF16     (Text);
 wchar_t * WideString = GetWideString(UTF16);
 delete[]  UTF16;
 return    WideString;
}
//------------------------------------------------------------------------------

wchar_t* UNICODE_CODEC::GetWideString(const unsigned char* Text){
 char16_t* UTF16      = GetUTF16     (Text);
 wchar_t * WideString = GetWideString(UTF16);
 delete[]  UTF16;
 return    WideString;
}
//------------------------------------------------------------------------------

wchar_t* UNICODE_CODEC::GetWideString(const char16_t* Text){
 int j;
 for(j = 0; Text[j]; j++);

 wchar_t* WideString = new wchar_t[j+1];

 for(j = 0; Text[j]; j++) WideString[j] = Text[j];
 WideString[j] = 0;

 return WideString;
}
//------------------------------------------------------------------------------

wchar_t* UNICODE_CODEC::GetWideString(const char32_t* Text){
 char16_t* UTF16      = GetUTF16     (Text);
 wchar_t * WideString = GetWideString(UTF16);
 delete[]  UTF16;
 return    WideString;
}
//------------------------------------------------------------------------------
