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

#include "ComboBox.h"
//------------------------------------------------------------------------------

COMBO_BOX::COMBO_BOX(
 int Left, int Width
): CONTROL(
  L"COMBOBOX",
  WS_BORDER | WS_TABSTOP | WS_VSCROLL | CBS_AUTOHSCROLL | CBS_DROPDOWNLIST,
  Left, -1,
  Width, 400
){
 ItemCount = 0;
}
//------------------------------------------------------------------------------

COMBO_BOX::~COMBO_BOX(){
}
//------------------------------------------------------------------------------

void COMBO_BOX::Clear(){
 ComboBox_ResetContent(Handle);
 ItemCount = 0;
}
//------------------------------------------------------------------------------

void COMBO_BOX::AddItem(const char* Item){
 UnicodeString Codec;
 Codec = Item;

 ComboBox_AddString(Handle, Codec.UTF16());
 if(!ItemCount) ComboBox_SetCurSel(Handle, 0);
 ItemCount++;
}
//------------------------------------------------------------------------------

void COMBO_BOX::GetItem(UnicodeString* Item){
 int      Length = ComboBox_GetTextLength(Handle);
 wchar_t* Text   = new wchar_t[Length+1];

 ComboBox_GetText(Handle, Text, Length+1);
 *Item = (char16*)Text;

 delete[] Text;
}
//------------------------------------------------------------------------------
