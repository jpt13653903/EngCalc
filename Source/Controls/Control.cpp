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

CONTROL::CONTROL(
 const wchar_t* Class, DWORD Style,
 int            Left , int   Top,
 int            Width, int   Height
){
 Handle = CreateWindow(
  Class,
  L"",
  WS_CHILD | WS_VISIBLE | Style,
  Left, Top,
  Width, Height,
  Window,
  0,
  Instance,
  0
 );
 SendMessage(Handle, WM_SETFONT, (WPARAM)Font, 0);
}
//------------------------------------------------------------------------------

CONTROL::~CONTROL(){
 DestroyWindow(Handle);
}
//------------------------------------------------------------------------------

void CONTROL::SetFocus(){
 ::SetFocus(Handle);
}
//------------------------------------------------------------------------------

void CONTROL::SetLeft(int Left){
 SetWindowPos (Handle, 0, Left, 0, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
}
//------------------------------------------------------------------------------

void CONTROL::Hide(){
 ShowWindow(Handle, SW_HIDE);
}
//------------------------------------------------------------------------------

void CONTROL::Show(){
 ShowWindow(Handle, SW_SHOW);
}
//------------------------------------------------------------------------------
