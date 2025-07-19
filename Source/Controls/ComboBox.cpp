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

ComboBox::ComboBox(
    int left, int width
): Control(
    L"COMBOBOX",
    WS_BORDER | WS_TABSTOP | WS_VSCROLL | CBS_AUTOHSCROLL | CBS_DROPDOWNLIST,
    left, -1,
    width, 400
){
    itemCount = 0;
}
//------------------------------------------------------------------------------

ComboBox::~ComboBox()
{
}
//------------------------------------------------------------------------------

void ComboBox::clear()
{
    ComboBox_ResetContent(handle);
    itemCount = 0;
}
//------------------------------------------------------------------------------

void ComboBox::addItem(const char* item)
{
    ComboBox_AddString(handle, (wchar_t*)(utfConverter.toUtf16(item).c_str()));
    if(!itemCount) ComboBox_SetCurSel(handle, 0);
    itemCount++;
}
//------------------------------------------------------------------------------

void ComboBox::getItem(std::string* item)
{
    int      length = ComboBox_GetTextLength(handle);
    wchar_t* text   = new wchar_t[length+1];

    ComboBox_GetText(handle, text, length+1);
    *item = utfConverter.toUtf8((char16_t*)text);

    delete[] text;
}
//------------------------------------------------------------------------------
