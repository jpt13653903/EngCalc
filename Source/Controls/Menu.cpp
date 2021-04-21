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

MENU::MENU(){
  Handle = CreatePopupMenu();

  // Create the menu items
  AppendMenu(Handle, MF_STRING, IDM_DEGREES        , L"Degrees\tCtrl G");
  AppendMenu(Handle, MF_STRING, IDM_RADIANS        , L"Radians\tCtrl R");
  AppendMenu(Handle, MF_SEPARATOR, 0, 0);
  AppendMenu(Handle, MF_STRING, IDM_DECIMAL        , L"Decimal\tCtrl D");
  AppendMenu(Handle, MF_STRING, IDM_HEX            , L"Hex\tCtrl H");
  AppendMenu(Handle, MF_STRING, IDM_BINARY         , L"Binary\tCtrl B");
  AppendMenu(Handle, MF_SEPARATOR, 0, 0);
  AppendMenu(Handle, MF_STRING, IDM_NORMAL         , L"Normal");
  AppendMenu(Handle, MF_STRING, IDM_ENGINEERING    , L"Engineering");
  AppendMenu(Handle, MF_STRING, IDM_FEET_INCHES    , L"Feet && Inches");
  AppendMenu(Handle, MF_STRING, IDM_DEG_MIN_SEC    , L"Deg, Min && Sec");
  AppendMenu(Handle, MF_SEPARATOR, 0, 0);
  AppendMenu(Handle, MF_STRING, IDM_GROUP_DIGITS   , L"Group Digits\tCtrl S");
  AppendMenu(Handle, MF_SEPARATOR, 0, 0);
  AppendMenu(Handle, MF_STRING, IDM_INCREASE_DIGITS, L"Increase Digits\tCtrl +");
  AppendMenu(Handle, MF_STRING, IDM_DECREASE_DIGITS, L"Decrease Digits\tCtrl -");
  AppendMenu(Handle, MF_SEPARATOR, 0, 0);
  AppendMenu(Handle, MF_STRING, IDM_DOT_DECIMALS   , L"Dot Decimals\tCtrl .");
  AppendMenu(Handle, MF_STRING, IDM_COMMA_DECIMALS , L"Comma Decimals\tCtrl ,");
  AppendMenu(Handle, MF_SEPARATOR, 0, 0);
  AppendMenu(Handle, MF_SEPARATOR, 0, 0);
  AppendMenu(Handle, MF_STRING, IDM_ALWAYS_ON_TOP  , L"Always on top\tCtrl T");
  AppendMenu(Handle, MF_STRING, IDM_CONVERTER      , L"Converter\tCtrl Z");
  AppendMenu(Handle, MF_SEPARATOR, 0, 0);
  AppendMenu(Handle, MF_STRING, IDM_MINIMIZE       , L"Minimize\tCtrl M");
  AppendMenu(Handle, MF_STRING, IDM_EXIT           , L"Exit\tCtrl Q");
  AppendMenu(Handle, MF_SEPARATOR, 0, 0);
  AppendMenu(Handle, MF_STRING, IDM_MANUAL         , L"Online Manual");
  AppendMenu(Handle, MF_STRING, IDM_ABOUT          , L"About");

  // Set the default states;
  SetRadians      (true);
  SetConverter    (true);
  SetAlwaysOnTop  (true);
  SetDisplayMode  (DISPLAY_MODE::Engineering);
  SetGroupDigits  (true);
  SetDecimalFormat('.');
  SetFormat       (FORMAT::Decimal);

  // Create the keyboard shortcuts
  ACCEL Accelerators[16];

  int j = 0;

  Accelerators[j  ].fVirt = FCONTROL | FVIRTKEY;
  Accelerators[j  ].key   = 'G';
  Accelerators[j++].cmd   = IDM_DEGREES;

  Accelerators[j  ].fVirt = FCONTROL | FVIRTKEY;
  Accelerators[j  ].key   = 'R';
  Accelerators[j++].cmd   = IDM_RADIANS;

  Accelerators[j  ].fVirt = FCONTROL | FVIRTKEY;
  Accelerators[j  ].key   = 'D';
  Accelerators[j++].cmd   = IDM_DECIMAL;

  Accelerators[j  ].fVirt = FCONTROL | FVIRTKEY;
  Accelerators[j  ].key   = 'H';
  Accelerators[j++].cmd   = IDM_HEX;

  Accelerators[j  ].fVirt = FCONTROL | FVIRTKEY;
  Accelerators[j  ].key   = 'B';
  Accelerators[j++].cmd   = IDM_BINARY;

  Accelerators[j  ].fVirt = FCONTROL | FVIRTKEY;
  Accelerators[j  ].key   = 'S';
  Accelerators[j++].cmd   = IDM_GROUP_DIGITS;

  Accelerators[j  ].fVirt = FCONTROL | FVIRTKEY;
  Accelerators[j  ].key   = VK_OEM_PLUS;
  Accelerators[j++].cmd   = IDM_INCREASE_DIGITS;

  Accelerators[j  ].fVirt = FCONTROL | FVIRTKEY;
  Accelerators[j  ].key   = VK_ADD;
  Accelerators[j++].cmd   = IDM_INCREASE_DIGITS;

  Accelerators[j  ].fVirt = FCONTROL | FVIRTKEY;
  Accelerators[j  ].key   = VK_OEM_MINUS;
  Accelerators[j++].cmd   = IDM_DECREASE_DIGITS;

  Accelerators[j  ].fVirt = FCONTROL | FVIRTKEY;
  Accelerators[j  ].key   = VK_SUBTRACT;
  Accelerators[j++].cmd   = IDM_DECREASE_DIGITS;

  Accelerators[j  ].fVirt = FCONTROL | FVIRTKEY;
  Accelerators[j  ].key   = VK_OEM_PERIOD;
  Accelerators[j++].cmd   = IDM_DOT_DECIMALS;

  Accelerators[j  ].fVirt = FCONTROL | FVIRTKEY;
  Accelerators[j  ].key   = VK_OEM_COMMA;
  Accelerators[j++].cmd   = IDM_COMMA_DECIMALS;

  Accelerators[j  ].fVirt = FCONTROL | FVIRTKEY;
  Accelerators[j  ].key   = 'T';
  Accelerators[j++].cmd   = IDM_ALWAYS_ON_TOP;

  Accelerators[j  ].fVirt = FCONTROL | FVIRTKEY;
  Accelerators[j  ].key   = 'Z';
  Accelerators[j++].cmd   = IDM_CONVERTER;

  Accelerators[j  ].fVirt = FCONTROL | FVIRTKEY;
  Accelerators[j  ].key   = 'M';
  Accelerators[j++].cmd   = IDM_MINIMIZE;

  Accelerators[j  ].fVirt = FCONTROL | FVIRTKEY;
  Accelerators[j  ].key   = 'Q';
  Accelerators[j++].cmd   = IDM_EXIT;

  AcceleratorTable = CreateAcceleratorTable(Accelerators, j);
}
//------------------------------------------------------------------------------

MENU::~MENU(){
  DestroyAcceleratorTable(AcceleratorTable);
  DestroyMenu            (Handle);
}
//------------------------------------------------------------------------------

void MENU::SetRadians(bool Value){
  MENUITEMINFO MenuInfo;
  MenuInfo.cbSize = sizeof(MENUITEMINFO);
  MenuInfo.fMask  = MIIM_STATE;
  MenuInfo.fState = MFS_UNCHECKED;

  SetMenuItemInfo(Handle, IDM_RADIANS, 0, &MenuInfo);
  SetMenuItemInfo(Handle, IDM_DEGREES, 0, &MenuInfo);

  MenuInfo.fState = MFS_CHECKED;
  if(Value) SetMenuItemInfo(Handle, IDM_RADIANS, 0, &MenuInfo);
  else      SetMenuItemInfo(Handle, IDM_DEGREES, 0, &MenuInfo);

  Radians = Value;
}
//------------------------------------------------------------------------------

void MENU::SetConverter(bool Value){
  MENUITEMINFO MenuInfo;
  MenuInfo.cbSize     = sizeof(MENUITEMINFO);
  MenuInfo.fMask      = MIIM_STRING;
  MenuInfo.fType      = MFT_STRING;
  MenuInfo.cch        = 32;

  MenuInfo.dwTypeData = new wchar_t[32];

  if(Value) wcscpy(MenuInfo.dwTypeData, L"Calculator\tCtrl Z");
  else      wcscpy(MenuInfo.dwTypeData, L"Converter\tCtrl Z" );

  SetMenuItemInfo(Handle, IDM_CONVERTER, 0, &MenuInfo);

  delete[] MenuInfo.dwTypeData;

  Converter = Value;
}
//------------------------------------------------------------------------------

void MENU::SetAlwaysOnTop(bool Value){
  MENUITEMINFO MenuInfo;
  MenuInfo.cbSize = sizeof(MENUITEMINFO);
  MenuInfo.fMask  = MIIM_STATE;

  if(Value) MenuInfo.fState = MFS_CHECKED;
  else      MenuInfo.fState = MFS_UNCHECKED;;

  SetMenuItemInfo(Handle, IDM_ALWAYS_ON_TOP, 0, &MenuInfo);

  AlwaysOnTop = Value;
}
//------------------------------------------------------------------------------

void MENU::SetDisplayMode(DISPLAY_MODE Value){
  MENUITEMINFO MenuInfo;
  MenuInfo.cbSize = sizeof(MENUITEMINFO);
  MenuInfo.fMask  = MIIM_STATE;
  MenuInfo.fState = MFS_UNCHECKED;

  SetMenuItemInfo(Handle, IDM_NORMAL     , 0, &MenuInfo);
  SetMenuItemInfo(Handle, IDM_ENGINEERING, 0, &MenuInfo);
  SetMenuItemInfo(Handle, IDM_FEET_INCHES, 0, &MenuInfo);
  SetMenuItemInfo(Handle, IDM_DEG_MIN_SEC, 0, &MenuInfo);

  MenuInfo.fState = MFS_CHECKED;
  SetMenuItemInfo(Handle, (int)Value, 0, &MenuInfo);

  DisplayMode = Value;
}
//------------------------------------------------------------------------------

void MENU::SetGroupDigits(bool Value){
  MENUITEMINFO MenuInfo;
  MenuInfo.cbSize = sizeof(MENUITEMINFO);
  MenuInfo.fMask  = MIIM_STATE;

  if(Value) MenuInfo.fState = MFS_CHECKED;
  else      MenuInfo.fState = MFS_UNCHECKED;;

  SetMenuItemInfo(Handle, IDM_GROUP_DIGITS, 0, &MenuInfo);

  GroupDigits = Value;
}
//------------------------------------------------------------------------------

void MENU::SetDecimalFormat(char Value){
  MENUITEMINFO MenuInfo;
  MenuInfo.cbSize = sizeof(MENUITEMINFO);
  MenuInfo.fMask  = MIIM_STATE;
  MenuInfo.fState = MFS_UNCHECKED;

  SetMenuItemInfo(Handle, IDM_DOT_DECIMALS  , 0, &MenuInfo);
  SetMenuItemInfo(Handle, IDM_COMMA_DECIMALS, 0, &MenuInfo);

  MenuInfo.fState = MFS_CHECKED;
  switch(Value){
    case '.':
      SetMenuItemInfo(Handle, IDM_DOT_DECIMALS, 0, &MenuInfo);
      break;

    case ',':
    default:
      SetMenuItemInfo(Handle, IDM_COMMA_DECIMALS, 0, &MenuInfo);
      break;
  }

  DecimalFormat = Value;
}
//------------------------------------------------------------------------------

void MENU::SetFormat(FORMAT Value){
  MENUITEMINFO MenuInfo;
  MenuInfo.cbSize = sizeof(MENUITEMINFO);
  MenuInfo.fMask  = MIIM_STATE;
  MenuInfo.fState = MFS_UNCHECKED;

  SetMenuItemInfo(Handle, IDM_BINARY , 0, &MenuInfo);
  SetMenuItemInfo(Handle, IDM_DECIMAL, 0, &MenuInfo);
  SetMenuItemInfo(Handle, IDM_HEX    , 0, &MenuInfo);

  MenuInfo.fState = MFS_CHECKED;
  switch(Value){
    case FORMAT::Binary:
      SetMenuItemInfo(Handle, IDM_BINARY, 0, &MenuInfo);
      break;

    case FORMAT::Hexadecimal:
      SetMenuItemInfo(Handle, IDM_HEX, 0, &MenuInfo);
      break;

    case FORMAT::Decimal:
    default:
      SetMenuItemInfo(Handle, IDM_DECIMAL, 0, &MenuInfo);
      break;
  }

  Format = Value;
}
//------------------------------------------------------------------------------

bool MENU::GetRadians(){
  return Radians;
}
//------------------------------------------------------------------------------

bool MENU::GetConverter(){
  return Converter;
}
//------------------------------------------------------------------------------

bool MENU::GetAlwaysOnTop(){
  return AlwaysOnTop;
}
//------------------------------------------------------------------------------

MENU::DISPLAY_MODE MENU::GetDisplayMode(){
  return DisplayMode;
}
//------------------------------------------------------------------------------

bool MENU::GetGroupDigits(){
  return GroupDigits;
}
//------------------------------------------------------------------------------

char MENU::GetDecimalFormat(){
  return DecimalFormat;
}
//------------------------------------------------------------------------------

MENU::FORMAT MENU::GetFormat(){
  return Format;
}
//------------------------------------------------------------------------------

void MENU::Show(){
  POINT Mouse;
  GetCursorPos(&Mouse);
  TrackPopupMenu(
    Handle,
    TPM_LEFTALIGN | TPM_TOPALIGN,
    Mouse.x, Mouse.y,
    0,
    Window,
    0
  );
}
//------------------------------------------------------------------------------
