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

enum MENU_Enum{
  IDM_DEGREES,
  IDM_RADIANS,
  IDM_DECIMAL,
  IDM_HEX,
  IDM_BINARY,
  IDM_NORMAL,
  IDM_ENGINEERING,
  IDM_FEET_INCHES,
  IDM_DEG_MIN_SEC,
  IDM_GROUP_DIGITS,
  IDM_INCREASE_DIGITS,
  IDM_DECREASE_DIGITS,
  IDM_DOT_DECIMALS,
  IDM_COMMA_DECIMALS,
  IDM_ALWAYS_ON_TOP,
  IDM_CONVERTER,
  IDM_MINIMIZE,
  IDM_EXIT,
  IDM_MANUAL,
  IDM_ABOUT
};
//------------------------------------------------------------------------------

class MENU{
  public:
    enum class FORMAT {Decimal, Hexadecimal, Binary};

    enum class DISPLAY_MODE{
      Normal      = IDM_NORMAL,
      Engineering = IDM_ENGINEERING,
      FeetInches  = IDM_FEET_INCHES,
      DegMinSec   = IDM_DEG_MIN_SEC
    };

  private:
    bool         Radians;
    bool         Converter;
    bool         AlwaysOnTop;
    DISPLAY_MODE DisplayMode;
    bool         GroupDigits;
    char         DecimalFormat;
    FORMAT       Format;

  public:
    HMENU  Handle;
    HACCEL AcceleratorTable;

    MENU();
   ~MENU();

    void SetRadians      (bool         Value);
    void SetConverter    (bool         Value);
    void SetAlwaysOnTop  (bool         Value);
    void SetDisplayMode  (DISPLAY_MODE Value);
    void SetGroupDigits  (bool         Value);
    void SetDecimalFormat(char         Value);
    void SetFormat       (FORMAT       Value);

    bool         GetRadians      ();
    bool         GetConverter    ();
    bool         GetAlwaysOnTop  ();
    DISPLAY_MODE GetDisplayMode  ();
    bool         GetGroupDigits  ();
    char         GetDecimalFormat();
    FORMAT       GetFormat       ();

    void Show();
};
//------------------------------------------------------------------------------

#endif
//------------------------------------------------------------------------------
