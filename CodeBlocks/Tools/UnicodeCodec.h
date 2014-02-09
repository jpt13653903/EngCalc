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

#ifndef UnicodeCodec_h
#define UnicodeCodec_h
//------------------------------------------------------------------------------

//#include <uchar.h>
#define char16_t unsigned short
#define char32_t unsigned int
//------------------------------------------------------------------------------

struct UNICODE_CODEC{
 char    * GetUTF8      (const wchar_t      * Text);
 char    * GetUTF8      (const char16_t     * Text);
 char    * GetUTF8      (const char32_t     * Text);

 char16_t* GetUTF16     (const          char* Text);
 char16_t* GetUTF16     (const unsigned char* Text);
 char16_t* GetUTF16     (const wchar_t      * Text);
 char16_t* GetUTF16     (const char32_t     * Text);

 char32_t* GetUnicode   (const          char* Text);
 char32_t* GetUnicode   (const unsigned char* Text);
 char32_t* GetUnicode   (const wchar_t      * Text);
 char32_t* GetUnicode   (const char16_t     * Text);

 wchar_t * GetWideString(const          char* Text);
 wchar_t * GetWideString(const unsigned char* Text);
 wchar_t * GetWideString(const char16_t     * Text);
 wchar_t * GetWideString(const char32_t     * Text);
};
//------------------------------------------------------------------------------

#endif
//------------------------------------------------------------------------------
