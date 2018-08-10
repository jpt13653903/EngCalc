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

#ifndef Menu_h
#define Menu_h
//------------------------------------------------------------------------------

#include "Global.h"
//------------------------------------------------------------------------------

#define IDM_DEGREES        0
#define IDM_RADIANS        1
#define IDM_DECIMAL        2
#define IDM_HEX            3
#define IDM_BINARY         4
#define IDM_NORMAL         5
#define IDM_ENGINEERING    6
#define IDM_GROUP_DIGITS   7
#define IDM_ALWAYS_ON_TOP  8
#define IDM_CONVERTER      9
#define IDM_MINIMIZE      10
#define IDM_EXIT          11
#define IDM_MANUAL        12
#define IDM_ABOUT         13
//------------------------------------------------------------------------------

class MENU{
  public:
    enum FORMAT{Decimal, Hexadecimal, Binary};

  private:
    bool   Radians;
    bool   Converter;
    bool   AlwaysOnTop;
    bool   Engineering;
    bool   GroupDigits;
    FORMAT Format;

  public:
    HMENU  Handle;
    HACCEL AcceleratorTable;

    MENU();
   ~MENU();

    void SetRadians    (bool   Value);
    void SetConverter  (bool   Value);
    void SetAlwaysOnTop(bool   Value);
    void SetEngineering(bool   Value);
    void SetGroupDigits(bool   Value);
    void SetFormat     (FORMAT Value);

    bool   GetRadians    ();
    bool   GetConverter  ();
    bool   GetAlwaysOnTop();
    bool   GetEngineering();
    bool   GetGroupDigits();
    FORMAT GetFormat     ();

    void Show();
};
//------------------------------------------------------------------------------

#endif
//------------------------------------------------------------------------------
