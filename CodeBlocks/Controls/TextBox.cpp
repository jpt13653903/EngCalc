//==============================================================================
// Copyright (C) John-Philip Taylor
// jpt13653903@gmail.com
//
// This file is part of Engineering Calculator
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

#include "TextBox.h"
//------------------------------------------------------------------------------

TEXT_BOX::TEXT_BOX(
 int Left, int Width, bool ReadOnly
): CONTROL(
 L"EDIT", WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL, Left, 0, Width, 21
){
 if(ReadOnly) SendMessage(Handle, EM_SETREADONLY, 1, 0);
}
//------------------------------------------------------------------------------

TEXT_BOX::~TEXT_BOX(){
}
//------------------------------------------------------------------------------

void TEXT_BOX::SetText(const char* String){
 UNICODE_CODEC Codec;

 wchar_t* WideString = Codec.GetWideString(String);

 Edit_SetText(Handle, WideString);

 delete[] WideString;
}
//------------------------------------------------------------------------------

void TEXT_BOX::GetText(STRING* String){
 int      Length = Edit_GetTextLength(Handle);
 wchar_t* Text   = new wchar_t[Length+1];

 Edit_GetText(Handle, Text, Length+1);
 String->Set(Text);

 delete[] Text;
}
//------------------------------------------------------------------------------
