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

TextBox::TextBox(
    int left, int width, bool readOnly
): Control(
    L"EDIT", WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL, left, 0, width, 21
){
    if(readOnly) SendMessage(handle, EM_SETREADONLY, 1, 0);
}
//------------------------------------------------------------------------------

TextBox::~TextBox()
{
}
//------------------------------------------------------------------------------

void TextBox::setText(const char* string)
{
    Edit_SetText(handle, (wchar_t*)(utfConverter.toUtf16(string).c_str()));
}
//------------------------------------------------------------------------------

void TextBox::getText(std::string* string)
{
    int      length = Edit_GetTextLength(handle);
    wchar_t* text   = new wchar_t[length+1];

    Edit_GetText(handle, text, length+1);
    *string = utfConverter.toUtf8((char16_t*)text);

    delete[] text;
}
//------------------------------------------------------------------------------
