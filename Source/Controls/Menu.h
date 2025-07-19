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
    IDM_DEGREES = 10, // Skip the Enter (1) and Escape (2) constants
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

class Menu{
    public:
        enum class Format {Decimal, Hexadecimal, Binary};

        enum class DisplayMode{
            Normal      = IDM_NORMAL,
            Engineering = IDM_ENGINEERING,
            FeetInches  = IDM_FEET_INCHES,
            DegMinSec   = IDM_DEG_MIN_SEC
        };

    private:
        bool        radians;
        bool        converter;
        bool        alwaysOnTop;
        DisplayMode displayMode;
        bool        groupDigits;
        char        decimalFormat;
        Format      format;

    public:
        HMENU  handle;
        HACCEL acceleratorTable;

        Menu();
      ~Menu();

        void setRadians      (bool        value);
        void setConverter    (bool        value);
        void setAlwaysOnTop  (bool        value);
        void setDisplayMode  (DisplayMode value);
        void setGroupDigits  (bool        value);
        void setDecimalFormat(char        value);
        void setFormat       (Format      value);

        bool        getRadians      ();
        bool        getConverter    ();
        bool        getAlwaysOnTop  ();
        DisplayMode getDisplayMode  ();
        bool        getGroupDigits  ();
        char        getDecimalFormat();
        Format      getFormat       ();

        void show();
};
//------------------------------------------------------------------------------

#endif
//------------------------------------------------------------------------------
