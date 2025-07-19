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

#include "Menu.h"
//------------------------------------------------------------------------------

Menu::Menu()
{
    handle = CreatePopupMenu();

    // Create the menu items
    AppendMenu(handle, MF_STRING, IDM_DEGREES        , L"Degrees\tCtrl G");
    AppendMenu(handle, MF_STRING, IDM_RADIANS        , L"Radians\tCtrl R");
    AppendMenu(handle, MF_SEPARATOR, 0, 0);
    AppendMenu(handle, MF_STRING, IDM_DECIMAL        , L"Decimal\tCtrl D");
    AppendMenu(handle, MF_STRING, IDM_HEX            , L"Hex\tCtrl H");
    AppendMenu(handle, MF_STRING, IDM_BINARY         , L"Binary\tCtrl B");
    AppendMenu(handle, MF_SEPARATOR, 0, 0);
    AppendMenu(handle, MF_STRING, IDM_NORMAL         , L"Normal");
    AppendMenu(handle, MF_STRING, IDM_ENGINEERING    , L"Engineering");
    AppendMenu(handle, MF_STRING, IDM_FEET_INCHES    , L"Feet && Inches");
    AppendMenu(handle, MF_STRING, IDM_DEG_MIN_SEC    , L"Deg, Min && Sec");
    AppendMenu(handle, MF_SEPARATOR, 0, 0);
    AppendMenu(handle, MF_STRING, IDM_GROUP_DIGITS   , L"Group Digits\tCtrl S");
    AppendMenu(handle, MF_SEPARATOR, 0, 0);
    AppendMenu(handle, MF_STRING, IDM_INCREASE_DIGITS, L"Increase Digits\tCtrl +");
    AppendMenu(handle, MF_STRING, IDM_DECREASE_DIGITS, L"Decrease Digits\tCtrl -");
    AppendMenu(handle, MF_SEPARATOR, 0, 0);
    AppendMenu(handle, MF_STRING, IDM_DOT_DECIMALS   , L"Dot Decimals\tCtrl .");
    AppendMenu(handle, MF_STRING, IDM_COMMA_DECIMALS , L"Comma Decimals\tCtrl ,");
    AppendMenu(handle, MF_SEPARATOR, 0, 0);
    AppendMenu(handle, MF_STRING, IDM_ALWAYS_ON_TOP  , L"Always on top\tCtrl T");
    AppendMenu(handle, MF_STRING, IDM_CONVERTER      , L"Converter\tCtrl Z");
    AppendMenu(handle, MF_SEPARATOR, 0, 0);
    AppendMenu(handle, MF_STRING, IDM_MINIMIZE       , L"Minimize\tCtrl M");
    AppendMenu(handle, MF_STRING, IDM_EXIT           , L"Exit\tCtrl Q");
    AppendMenu(handle, MF_SEPARATOR, 0, 0);
    AppendMenu(handle, MF_STRING, IDM_MANUAL         , L"Online Manual");
    AppendMenu(handle, MF_STRING, IDM_ABOUT          , L"About");

    // set the default states;
    setRadians      (true);
    setConverter    (true);
    setAlwaysOnTop  (true);
    setDisplayMode  (DisplayMode::Engineering);
    setGroupDigits  (true);
    setDecimalFormat('.');
    setFormat       (Format::Decimal);

    // Create the keyboard shortcuts
    ACCEL accelerators[16];

    int j = 0;

    accelerators[j  ].fVirt = FCONTROL | FVIRTKEY;
    accelerators[j  ].key   = 'G';
    accelerators[j++].cmd   = IDM_DEGREES;

    accelerators[j  ].fVirt = FCONTROL | FVIRTKEY;
    accelerators[j  ].key   = 'R';
    accelerators[j++].cmd   = IDM_RADIANS;

    accelerators[j  ].fVirt = FCONTROL | FVIRTKEY;
    accelerators[j  ].key   = 'D';
    accelerators[j++].cmd   = IDM_DECIMAL;

    accelerators[j  ].fVirt = FCONTROL | FVIRTKEY;
    accelerators[j  ].key   = 'H';
    accelerators[j++].cmd   = IDM_HEX;

    accelerators[j  ].fVirt = FCONTROL | FVIRTKEY;
    accelerators[j  ].key   = 'B';
    accelerators[j++].cmd   = IDM_BINARY;

    accelerators[j  ].fVirt = FCONTROL | FVIRTKEY;
    accelerators[j  ].key   = 'S';
    accelerators[j++].cmd   = IDM_GROUP_DIGITS;

    accelerators[j  ].fVirt = FCONTROL | FVIRTKEY;
    accelerators[j  ].key   = VK_OEM_PLUS;
    accelerators[j++].cmd   = IDM_INCREASE_DIGITS;

    accelerators[j  ].fVirt = FCONTROL | FVIRTKEY;
    accelerators[j  ].key   = VK_ADD;
    accelerators[j++].cmd   = IDM_INCREASE_DIGITS;

    accelerators[j  ].fVirt = FCONTROL | FVIRTKEY;
    accelerators[j  ].key   = VK_OEM_MINUS;
    accelerators[j++].cmd   = IDM_DECREASE_DIGITS;

    accelerators[j  ].fVirt = FCONTROL | FVIRTKEY;
    accelerators[j  ].key   = VK_SUBTRACT;
    accelerators[j++].cmd   = IDM_DECREASE_DIGITS;

    accelerators[j  ].fVirt = FCONTROL | FVIRTKEY;
    accelerators[j  ].key   = VK_OEM_PERIOD;
    accelerators[j++].cmd   = IDM_DOT_DECIMALS;

    accelerators[j  ].fVirt = FCONTROL | FVIRTKEY;
    accelerators[j  ].key   = VK_OEM_COMMA;
    accelerators[j++].cmd   = IDM_COMMA_DECIMALS;

    accelerators[j  ].fVirt = FCONTROL | FVIRTKEY;
    accelerators[j  ].key   = 'T';
    accelerators[j++].cmd   = IDM_ALWAYS_ON_TOP;

    accelerators[j  ].fVirt = FCONTROL | FVIRTKEY;
    accelerators[j  ].key   = 'Z';
    accelerators[j++].cmd   = IDM_CONVERTER;

    accelerators[j  ].fVirt = FCONTROL | FVIRTKEY;
    accelerators[j  ].key   = 'M';
    accelerators[j++].cmd   = IDM_MINIMIZE;

    accelerators[j  ].fVirt = FCONTROL | FVIRTKEY;
    accelerators[j  ].key   = 'Q';
    accelerators[j++].cmd   = IDM_EXIT;

    acceleratorTable = CreateAcceleratorTable(accelerators, j);
}
//------------------------------------------------------------------------------

Menu::~Menu()
{
    DestroyAcceleratorTable(acceleratorTable);
    DestroyMenu            (handle);
}
//------------------------------------------------------------------------------

void Menu::setRadians(bool value)
{
    MENUITEMINFO menuInfo;
    menuInfo.cbSize = sizeof(MENUITEMINFO);
    menuInfo.fMask  = MIIM_STATE;
    menuInfo.fState = MFS_UNCHECKED;

    SetMenuItemInfo(handle, IDM_RADIANS, 0, &menuInfo);
    SetMenuItemInfo(handle, IDM_DEGREES, 0, &menuInfo);

    menuInfo.fState = MFS_CHECKED;
    if(value) SetMenuItemInfo(handle, IDM_RADIANS, 0, &menuInfo);
    else      SetMenuItemInfo(handle, IDM_DEGREES, 0, &menuInfo);

    radians = value;
}
//------------------------------------------------------------------------------

void Menu::setConverter(bool value)
{
    MENUITEMINFO menuInfo;
    menuInfo.cbSize     = sizeof(MENUITEMINFO);
    menuInfo.fMask      = MIIM_STRING;
    menuInfo.fType      = MFT_STRING;
    menuInfo.cch        = 32;

    menuInfo.dwTypeData = new wchar_t[32];

    if(value) wcscpy(menuInfo.dwTypeData, L"Calculator\tCtrl Z");
    else      wcscpy(menuInfo.dwTypeData, L"Converter\tCtrl Z" );

    SetMenuItemInfo(handle, IDM_CONVERTER, 0, &menuInfo);

    delete[] menuInfo.dwTypeData;

    converter = value;
}
//------------------------------------------------------------------------------

void Menu::setAlwaysOnTop(bool value)
{
    MENUITEMINFO menuInfo;
    menuInfo.cbSize = sizeof(MENUITEMINFO);
    menuInfo.fMask  = MIIM_STATE;

    if(value) menuInfo.fState = MFS_CHECKED;
    else      menuInfo.fState = MFS_UNCHECKED;;

    SetMenuItemInfo(handle, IDM_ALWAYS_ON_TOP, 0, &menuInfo);

    alwaysOnTop = value;
}
//------------------------------------------------------------------------------

void Menu::setDisplayMode(DisplayMode value)
{
    MENUITEMINFO menuInfo;
    menuInfo.cbSize = sizeof(MENUITEMINFO);
    menuInfo.fMask  = MIIM_STATE;
    menuInfo.fState = MFS_UNCHECKED;

    SetMenuItemInfo(handle, IDM_NORMAL     , 0, &menuInfo);
    SetMenuItemInfo(handle, IDM_ENGINEERING, 0, &menuInfo);
    SetMenuItemInfo(handle, IDM_FEET_INCHES, 0, &menuInfo);
    SetMenuItemInfo(handle, IDM_DEG_MIN_SEC, 0, &menuInfo);

    menuInfo.fState = MFS_CHECKED;
    SetMenuItemInfo(handle, (int)value, 0, &menuInfo);

    displayMode = value;
}
//------------------------------------------------------------------------------

void Menu::setGroupDigits(bool value)
{
    MENUITEMINFO menuInfo;
    menuInfo.cbSize = sizeof(MENUITEMINFO);
    menuInfo.fMask  = MIIM_STATE;

    if(value) menuInfo.fState = MFS_CHECKED;
    else      menuInfo.fState = MFS_UNCHECKED;;

    SetMenuItemInfo(handle, IDM_GROUP_DIGITS, 0, &menuInfo);

    groupDigits = value;
}
//------------------------------------------------------------------------------

void Menu::setDecimalFormat(char value)
{
    MENUITEMINFO menuInfo;
    menuInfo.cbSize = sizeof(MENUITEMINFO);
    menuInfo.fMask  = MIIM_STATE;
    menuInfo.fState = MFS_UNCHECKED;

    SetMenuItemInfo(handle, IDM_DOT_DECIMALS  , 0, &menuInfo);
    SetMenuItemInfo(handle, IDM_COMMA_DECIMALS, 0, &menuInfo);

    menuInfo.fState = MFS_CHECKED;
    switch(value){
        case '.':
            SetMenuItemInfo(handle, IDM_DOT_DECIMALS, 0, &menuInfo);
            break;

        case ',':
        default:
            SetMenuItemInfo(handle, IDM_COMMA_DECIMALS, 0, &menuInfo);
            break;
    }

    decimalFormat = value;
}
//------------------------------------------------------------------------------

void Menu::setFormat(Format value)
{
    MENUITEMINFO menuInfo;
    menuInfo.cbSize = sizeof(MENUITEMINFO);
    menuInfo.fMask  = MIIM_STATE;
    menuInfo.fState = MFS_UNCHECKED;

    SetMenuItemInfo(handle, IDM_BINARY , 0, &menuInfo);
    SetMenuItemInfo(handle, IDM_DECIMAL, 0, &menuInfo);
    SetMenuItemInfo(handle, IDM_HEX    , 0, &menuInfo);

    menuInfo.fState = MFS_CHECKED;
    switch(value){
        case Format::Binary:
            SetMenuItemInfo(handle, IDM_BINARY, 0, &menuInfo);
            break;

        case Format::Hexadecimal:
            SetMenuItemInfo(handle, IDM_HEX, 0, &menuInfo);
            break;

        case Format::Decimal:
        default:
            SetMenuItemInfo(handle, IDM_DECIMAL, 0, &menuInfo);
            break;
    }

    format = value;
}
//------------------------------------------------------------------------------

bool Menu::getRadians()
{
    return radians;
}
//------------------------------------------------------------------------------

bool Menu::getConverter()
{
    return converter;
}
//------------------------------------------------------------------------------

bool Menu::getAlwaysOnTop()
{
    return alwaysOnTop;
}
//------------------------------------------------------------------------------

Menu::DisplayMode Menu::getDisplayMode()
{
    return displayMode;
}
//------------------------------------------------------------------------------

bool Menu::getGroupDigits()
{
    return groupDigits;
}
//------------------------------------------------------------------------------

char Menu::getDecimalFormat()
{
    return decimalFormat;
}
//------------------------------------------------------------------------------

Menu::Format Menu::getFormat()
{
    return format;
}
//------------------------------------------------------------------------------

void Menu::show()
{
    POINT mouse;
    GetCursorPos(&mouse);
    TrackPopupMenu(
        handle,
        TPM_LEFTALIGN | TPM_TOPALIGN,
        mouse.x, mouse.y,
        0,
        window,
        0
    );
}
//------------------------------------------------------------------------------
