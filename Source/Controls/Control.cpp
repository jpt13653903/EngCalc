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

#include "Control.h"
//------------------------------------------------------------------------------

Control::Control(
    const wchar_t* windowClass, DWORD windowStyle,
    int left , int top,
    int width, int height
){
    handle = CreateWindow(
        windowClass,
        L"",
        WS_CHILD | WS_VISIBLE | windowStyle,
        left, top,
        width, height,
        window,
        0,
        instance,
        0
    );
    SendMessage(handle, WM_SETFONT, (WPARAM)font, 0);
}
//------------------------------------------------------------------------------

Control::~Control()
{
    DestroyWindow(handle);
}
//------------------------------------------------------------------------------

void Control::setFocus()
{
    ::SetFocus(handle);
}
//------------------------------------------------------------------------------

void Control::setLeft(int left)
{
    SetWindowPos(handle, 0, left, 0, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
}
//------------------------------------------------------------------------------

void Control::hide()
{
    ShowWindow(handle, SW_HIDE);
}
//------------------------------------------------------------------------------

void Control::show()
{
    ShowWindow(handle, SW_SHOW);
}
//------------------------------------------------------------------------------
