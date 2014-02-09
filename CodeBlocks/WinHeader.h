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

#ifndef WinHeader_h
#define WinHeader_h
//------------------------------------------------------------------------------

// Set the minimum Windows version to Windows XP
#define  NTDDI_VERSION 0x05010000
#define  WINVER        0x0501
#define _WIN32_WINNT   WINVER
#define _WIN32_IE      WINVER
#define  UNICODE
//------------------------------------------------------------------------------

#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
//------------------------------------------------------------------------------

#include "Resources.h"
//------------------------------------------------------------------------------

#endif
//------------------------------------------------------------------------------
