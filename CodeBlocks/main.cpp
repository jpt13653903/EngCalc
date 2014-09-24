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

#include "main.h"
//------------------------------------------------------------------------------

int   MonitorCount = 0;
int   MonitorIndex = 0;
RECT* Monitors;

BOOL CALLBACK MonitorEnum(HMONITOR Handle, HDC DC, LPRECT Rect, LPARAM Data){
 if(MonitorIndex >= MonitorCount) return false;

 Monitors[MonitorIndex].left   = Rect->left;
 Monitors[MonitorIndex].top    = Rect->top;
 Monitors[MonitorIndex].right  = Rect->right;
 Monitors[MonitorIndex].bottom = Rect->bottom;

 MonitorIndex++;

 return true;
}
//------------------------------------------------------------------------------

// X and Y are client coordinates of the mouse pointer
void MoveWindow(int X, int Y){
 // Get the mouse position
 POINT Point;
 GetCursorPos(&Point);

 // If there are no monitors, something is seriously wrong
 MonitorCount = GetSystemMetrics(SM_CMONITORS);
 if(!MonitorCount) return;

 // Find the monitor metrics
 MonitorIndex = 0;
 Monitors     = new RECT[MonitorCount];
 EnumDisplayMonitors(0, 0, MonitorEnum, 0);

 // Find the screen on which the mouse is
 int j;
 for(j = 0; j < MonitorCount-1; j++){
  if(Point.x >= Monitors[j].left && Point.x <= Monitors[j].right  &&
     Point.y >= Monitors[j].top  && Point.y <= Monitors[j].bottom ){
   break;
  }
 }

 // Make the move
 Point.x -= X;
 Point.y -= Y;

 // Make sure the entire window is on the screen
 if(Point.x < Monitors[j].left)
    Point.x = Monitors[j].left;
 if(Point.y < Monitors[j].top )
    Point.y = Monitors[j].top ;
 if(Point.x > Monitors[j].right  - MainWidth)
    Point.x = Monitors[j].right  - MainWidth;
 if(Point.y > Monitors[j].bottom - 21)
    Point.y = Monitors[j].bottom - 21;

 // Apply the new position
 SetWindowPos(Window, 0, Point.x, Point.y, 0, 0, SWP_NOSIZE);

 // Clean up the memory allocation
 delete[] Monitors;
}
//------------------------------------------------------------------------------

struct CONVERSION{
 STRING FromUnit;
 STRING ToUnit;
 STRING Formula;
};
//------------------------------------------------------------------------------

static int CONVERSION_Compare(void* Left, void* Right){
 CONVERSION* left  = (CONVERSION*)Left;
 CONVERSION* right = (CONVERSION*)Right;

 int Result;

 Result = left->FromUnit.CompareNoCase(right->FromUnit);
 if(Result) return Result;

 Result = left->FromUnit.Compare(right->FromUnit);
 if(Result) return Result;

 Result = left->ToUnit.CompareNoCase(right->ToUnit);
 if(Result) return Result;

 return left->ToUnit.Compare(right->ToUnit);
}
//------------------------------------------------------------------------------

static int CONVERSION_Compare_FromUnit(void* Left, void* Right){
 CONVERSION* left  = (CONVERSION*)Left;
 CONVERSION* right = (CONVERSION*)Right;

 int Result;

 Result = left->FromUnit.CompareNoCase(right->FromUnit);
 if(Result) return Result;

 return left->FromUnit.Compare(right->FromUnit);
}
//------------------------------------------------------------------------------

void OnFromUnitClick(){
 CONVERSION  Key;
 CONVERSION* Conversion;
 FromUnit->GetItem(&Key.FromUnit);

 ToUnit->Clear();

 // Finds the first of the list of matches
 Conversions.Compare = CONVERSION_Compare_FromUnit;
  Conversion = (CONVERSION*)Conversions.Find(&Key);
 Conversions.Compare = CONVERSION_Compare;

 while(Conversion){
  if(Key.FromUnit != Conversion->FromUnit) break;

  ToUnit->AddItem(Conversion->ToUnit.UTF8());
  Conversion = (CONVERSION*)Conversions.Next();
 }
}
//------------------------------------------------------------------------------

void LoadConversions(){
 STRING      s;
 CONVERSION* Conversion;
 XML_Wrapper XML;

 // Read Converter.xml
 if(!XML.Load("..\\Converter.xml")) return;

 XML_Wrapper::ENTITY* Entity;
 Entity = XML.FindChild(XML.Root, "Conversion");
 while(Entity){
  Conversion = new CONVERSION;
  XML.ReadAttribute(Entity, "From"   , &Conversion->FromUnit);
  XML.ReadAttribute(Entity, "To"     , &Conversion->ToUnit  );
  XML.ReadAttribute(Entity, "Formula", &Conversion->Formula );
  if(
   Conversion->FromUnit.Length32() &&
   Conversion->ToUnit  .Length32() &&
   Conversion->Formula .Length32()
  ){
   Conversions.Insert(Conversion);
  }else{
   delete Conversion;
  }
  Entity = XML.NextChild(XML.Root, "Conversion");
 }

 // Add the conversions to the "FromUnit" combo-box.
 s = "";
 Conversion = (CONVERSION*)Conversions.First();
 while(Conversion){
  if(s != Conversion->FromUnit){
   FromUnit->AddItem(Conversion->FromUnit.UTF8());
   s = Conversion->FromUnit;
  }
  Conversion = (CONVERSION*)Conversions.Next();
 }

 if(Conversions.ItemCount()) OnFromUnitClick();
}
//------------------------------------------------------------------------------

bool Invalid(long double d, STRING* s){
 unsigned* p;

 // See the "Intel Architecture Software Developer's Manual,
 // Volume 1: Basic Architecture" for details on the floating point format
 p = (unsigned*)&d;
 if(((p[2] & 0x7FFF) == 0x7FFF) && (p[1] & 0x80000000)){
  if(p[2] & 0x8000){
   if(p[1] == 0x80000000) *s = "-Infinity";
   else                   *s = "Not a number";
  }else{
   *s = "Infinite";
  }
  return true;
 }
 return false;
}
//------------------------------------------------------------------------------

// These do not use the STRING::Set functions
// because a custom format is desired
void ToHex(long double d, STRING* s){
 int  exp, places;
 char c;
 bool spaces = Menu->GetGroupDigits();

 if(Invalid(d, s)) return;

 if(d == 0.0){
  *s = "0";
  return;
 }

 if(d < 0.0){
  *s = "-0x";
  d *= -1.0;
 }else{
  *s = "0x";
 }

 exp = 0;
 while(d >= 16.0){
  d  /= 16.0;
  exp++;
 }

 places = 0;
 while(places < 16 || exp >= 0){
  if(exp < 0 && d == 0.0) return;

  if(exp == -1){
   *s += '.';
  }else{
   if(spaces && places && ((exp+1) % 4 == 0)) *s += ' ';
  }

  c = floorl(d);

  if(c <= 9) *s += (char)(c + '0');
  else       *s += (char)(c + 'A' - 10);

  d -= c;
  d *= 16.0;

  places++;
  exp   --;
 }
}
//------------------------------------------------------------------------------

void ToBinary(long double d, STRING* s){
 int  exp, places;
 char c;
 bool spaces = Menu->GetGroupDigits();

 if(Invalid(d, s)) return;

 if(d == 0.0){
  *s = "0";
  return;
 }

 if(d < 0.0){
  *s = "-0b";
  d *= -1.0;
 }else{
  *s = "0b";
 }

 exp = 0;
 while(d >= 2.0){
  d  /= 2.0;
  exp++;
 }

 places = 0;
 while(places < 64 || exp >= 0){
  if(exp < 0 && d == 0.0) return;

  if(exp == -1){
   *s += '.';
  }else{
   if(spaces && places && ((exp+1) % 4 == 0)) *s += ' ';
  }

  c = floorl(d);

  *s += (char)(c + '0');

  d -= c;
  d *= 2.0;

  places++;
  exp   --;
 }
}
//------------------------------------------------------------------------------

void ToDecimal(long double d, STRING* s){
 int  j, exp, Exponent;
 bool spaces = Menu->GetGroupDigits();

 if(Invalid(d, s)) return;

 if(d == 0.0){
  *s = "0";
  return;
 }

 *s = "";
 bool Sign = false;
 if(d < 0.0){
  d *= -1.0;
  Sign = true;
 }

 Exponent = 0;
 if(Menu->GetEngineering()){
  while(d >= 1e3){
   d /= 1e3;
   Exponent += 3;
  }
  while(d < 1.0){
   d *= 1e3;
   Exponent -= 3;
  }

 }else{
  if(d > 1e6){
   while(d >= 10.0){
    d /= 10.0;
    Exponent++;
   }
  }else if(d < 1e-6){
   while(d < 1.0){
    d *= 10.0;
    Exponent--;
   }
  }
 }

 exp = 0;
 while(d >= 10.0){
  d  /= 10.0;
  exp++;
 }

 // Use 17 significant figures (limit of long double is about 19 figures)
 d    = roundl(d*powl(10.0, 17));
 exp -= 17;

 // Add trailing zeroes
 for(j = 0; j < exp; j++){
  if(spaces && j % 3 == 0) *s += ' ';
  *s += '0';
 }

 // Remove zeros after decimal point
 while(exp < 0){
  if(fmodl(d, 10.0) != 0.0) break;
  d = floorl(d / 10.0);
  exp++;
 }

 while(d > 0.0 || exp <= 0){
  if     (exp     == 0 && s->Length32()) *s += '.';
  else if(exp % 3 == 0 && spaces       ) *s += ' ';

  *s += (char)(fmodl(d, 10.0) + '0');
  d = floorl(d / 10.0);
  exp++;
 }

 if(Sign) *s += '-';

 // Reverse
 int     Length_32 = s->Length32();
 int l = Length_32 / 2;
 char32* Data_32   = s->UTF32   ();
 char32  c;

 for(j = 0; j < l; j++){
  c = Data_32[j];
  Data_32[j] = Data_32[Length_32-j-1];
  Data_32[Length_32-j-1] = c;
 }

 if(Exponent){
  if(spaces) *s += ' ';
  *s += 'e';
  *s += Exponent;
 }
}
//------------------------------------------------------------------------------

void Calculate(){
 STRING s, formula;

 Formula->GetText(&formula);
 if(!formula.Length32()){
  Result->SetText("");
  return;
 }

 long double result = Calc.Calculate(formula.UTF8());
 if(Menu->GetConverter()){
  CONVERSION  Key;
  CONVERSION* Conversion;
  FromUnit->GetItem(&Key.FromUnit);
  ToUnit  ->GetItem(&Key.ToUnit  );

  Conversion = (CONVERSION*)Conversions.Find(&Key);
  if(!Conversion) result = 0.0;
  else result = Calc.Calculate(Conversion->Formula.UTF8(), "x", result);
 }

 switch(Menu->GetFormat()){
  case MENU::Binary:
   ToBinary(result, &s);
   break;

  case MENU::Hexadecimal:
   ToHex(result, &s);
   break;

  default:
   ToDecimal(result, &s);
   break;
 }
 Result->SetText(s.UTF8());
}
//------------------------------------------------------------------------------

void OnAboutClick(){
 MessageBox(
  Window,
  L"Engineering Calculator, Version 1.4\n"
  L"Built on "__DATE__" at "__TIME__"\n"
  L"\n"
  L"Copyright (C) John-Philip Taylor\n"
  L"jpt13653903@gmail.com\n"
  L"\n"
  L"Developed using:\n"
  L"- Code::Blocks 13.12 <http://www.codeblocks.org/>\n"
  L"- MinGW (tdm-1) 4.8.1 <http://tdm-gcc.tdragon.net/>\n"
  L"\n"
  L"This program is free software: you can redistribute it and/or modify\n"
  L"it under the terms of the GNU General Public License as published by\n"
  L"the Free Software Foundation, either version 3 of the License, or\n"
  L"(at your option) any later version.\n"
  L"\n"
  L"This program is distributed in the hope that it will be useful,\n"
  L"but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
  L"MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"
  L"GNU General Public License for more details.\n"
  L"\n"
  L"You should have received a copy of the GNU General Public License\n"
  L"along with this program.  If not, see <http://www.gnu.org/licenses/>",
  L"About",
  MB_ICONINFORMATION
 );
}
//------------------------------------------------------------------------------

LRESULT CALLBACK WindowProcedure(
 HWND   Handle,
 UINT   Message,
 WPARAM wParam,
 LPARAM lParam
){
 static int X, Y;

 RECT   Rect;
 STRING s;

 switch(Message){
  case WM_CLOSE:
  case WM_DESTROY:
   PostQuitMessage(0);
   break;

  case WM_LBUTTONDOWN:
   X = GET_X_LPARAM(lParam);
   Y = GET_Y_LPARAM(lParam);
   SetCapture(Window);
   break;

  case WM_MOUSEMOVE:
   if(wParam & MK_LBUTTON) MoveWindow(X, Y);
   break;

  case WM_LBUTTONUP:
   ReleaseCapture();
   break;

  case WM_RBUTTONUP:
   Menu->Show();
   break;

  case NotifyMessage:
   switch(lParam){
    case WM_LBUTTONDOWN:
     ShowWindow         (Window, SW_SHOW); // Show if hidden
     SetForegroundWindow(Window);          // Activate if in the background
     SetFocus           (Formula->Handle); // Set user input to the formula
     break;

    case WM_RBUTTONUP:
     Menu->Show();
     break;

    default:
     break;
   }
   break;

  case WM_COMMAND:
   if((HWND)lParam == Formula->Handle && HIWORD(wParam) == EN_CHANGE){
    Calculate();

   }else if(
    (HWND) lParam  == FromUnit->Handle &&
    HIWORD(wParam) == CBN_SELCHANGE
   ){
    OnFromUnitClick();
    Calculate();

   }else if(
    (HWND) lParam  == ToUnit->Handle &&
    HIWORD(wParam) == CBN_SELCHANGE
   ){
    Calculate();

   }else if(lParam == 0){
    switch(LOWORD(wParam)){
     case IDM_DEGREES:
      Menu->SetRadians(false);
      Calc.Measure = CALCULATOR::Degrees;
      Calculate();
      break;

     case IDM_RADIANS:
      Menu->SetRadians(true);
      Calc.Measure = CALCULATOR::Radians;
      Calculate();
      break;

     case IDM_DECIMAL:
      Menu->SetFormat(MENU::Decimal);
      Calculate();
      break;

     case IDM_HEX:
      Menu->SetFormat(MENU::Hexadecimal);
      Calculate();
      break;

     case IDM_BINARY:
      Menu->SetFormat(MENU::Binary);
      Calculate();
      break;

     case IDM_NORMAL:
      Menu->SetEngineering(false);
      Calculate();
      break;

     case IDM_ENGINEERING:
      Menu->SetEngineering(true);
      Calculate();
      break;

     case IDM_GROUP_DIGITS:
      Menu->SetGroupDigits(!Menu->GetGroupDigits());
      Calculate();
      break;

     case IDM_ALWAYS_ON_TOP:
      LONG Style;
      Style = GetWindowLong(Window, GWL_EXSTYLE);
      if(Menu->GetAlwaysOnTop()){
       Style &= ~WS_EX_TOPMOST;
       Menu->SetAlwaysOnTop(false);
       SetWindowLong(Window, GWL_EXSTYLE, Style);
       SetWindowPos(
        Window,
        HWND_NOTOPMOST,
        0, 0, 0, 0,
        SWP_NOMOVE | SWP_NOSIZE
       );
      }else{
       Style |= WS_EX_TOPMOST;
       Menu->SetAlwaysOnTop(true);
       SetWindowLong(Window, GWL_EXSTYLE, Style);
       SetWindowPos(
        Window,
        HWND_TOPMOST,
        0, 0, 0, 0,
        SWP_NOMOVE | SWP_NOSIZE
       );
      }
      break;

     case IDM_CONVERTER:
      GetWindowRect(Window, &Rect);
      if(Menu->GetConverter()){
       MainWidth  = 440;
       Rect.left += 300;

       Label   ->SetLeft(210);
       Result  ->SetLeft(230);
       FromUnit->Hide();
       ToUnit  ->Hide();
       Menu    ->SetConverter(false);

      }else{
       MainWidth  = 740;
       Rect.left -= 300;

       FromUnit->Show();
       ToUnit  ->Show();
       Label   ->SetLeft(360);
       Result  ->SetLeft(380);
       Menu    ->SetConverter(true);
      }
      SetWindowPos(Window, 0, Rect.left, Rect.top, MainWidth, 21, 0);
      Result ->GetText(&s);
      Formula->SetText( s.UTF8());
      Calculate();
      break;

     case IDM_MINIMIZE:
      ShowWindow(Window, SW_HIDE);
      break;

     case IDM_EXIT:
      PostQuitMessage(0);
      break;

     case IDM_MANUAL:
      WinExec(
       "explorer.exe "
       "\"https://sourceforge.net/p/alwaysontopcalc/wiki/Home/\"",
       SW_MAXIMIZE
      );
      break;

     case IDM_ABOUT:
      OnAboutClick();
      break;
    }
   }
   break;

  default:
   return DefWindowProc(Handle, Message, wParam, lParam);
 }
 return 0;
}
//------------------------------------------------------------------------------

static HICON LoadIconSmall(WORD Icon){
 int    size = GetSystemMetrics(SM_CXSMICON);
 return (HICON)LoadImage(
  Instance,
  MAKEINTRESOURCE(Icon),
  IMAGE_ICON,
  size, size,
  LR_SHARED
 );
}
//------------------------------------------------------------------------------

int WINAPI WinMain(
 HINSTANCE hInstance,
 HINSTANCE hPrevInstance,
 LPSTR     CommandLine,
 int       CmdShow
){
 INITCOMMONCONTROLSEX icex;
 icex.dwSize = sizeof(icex);
 icex.dwICC  = 0xFFFF; // Load everything
 InitCommonControlsEx(&icex);

 // Make sure the "working folder" is correct
 int      j, q, i;
 wchar_t* FullCommandLine = GetCommandLineW();
 wchar_t* NewCurrentDirectory;

 for(j = 0; FullCommandLine[j]; j++);
 while(j && FullCommandLine[j] != '\\') j--;
 if(j){
  NewCurrentDirectory = new wchar_t[j+2];
  q = i = 0;
  if(FullCommandLine[q] == '"') q++;
  while(q <= j) NewCurrentDirectory[i++] = FullCommandLine[q++];
  NewCurrentDirectory[i] = 0;

  SetCurrentDirectoryW(NewCurrentDirectory);
  delete[] NewCurrentDirectory;
 }

 // Store the Instance handle in the global variable
 Instance = hInstance;

 // Register the Window Class
 WNDCLASSEX Class;
 Class.cbSize        = sizeof(WNDCLASSEX);
 Class.hInstance     = Instance;
 Class.lpszClassName = L"EngineeringCalculator";
 Class.lpfnWndProc   = WindowProcedure;
 Class.style         = 0;
 Class.hIcon         = LoadIcon(Instance, MAKEINTRESOURCE(MainIcon));
 Class.hIconSm       = LoadIconSmall(MainIcon);
 Class.hCursor       = LoadCursor(0, IDC_ARROW);
 Class.lpszMenuName  = 0;
 Class.cbClsExtra    = 0;
 Class.cbWndExtra    = 0;
 Class.hbrBackground = (HBRUSH)COLOR_WINDOW;

 if(!RegisterClassEx(&Class)) return 2;

 // Get the system font
 NONCLIENTMETRICS Metrics;
 Metrics.cbSize = sizeof(NONCLIENTMETRICS);
 SystemParametersInfo(SPI_GETNONCLIENTMETRICS, Metrics.cbSize, &Metrics, 0);
 Font = CreateFontIndirect(&Metrics.lfCaptionFont);

 // Create the window
 RECT Desktop;
 GetWindowRect(GetDesktopWindow(), &Desktop);

 MainWidth = 740;

 Window = CreateWindowEx(
  WS_EX_TOPMOST | WS_EX_TOOLWINDOW,
  L"EngineeringCalculator",
  L"Eng Calc",
  WS_POPUP,
  Desktop.right - MainWidth - RightSpace, 0,
  MainWidth, 21,
  HWND_DESKTOP,
  0,
  Instance,
  0
 );
 SendMessage(Window, WM_SETFONT, (WPARAM)Font, 0);

 // Add a notification icon
 NOTIFYICONDATA NotifyIcon;
 ZeroMemory(&NotifyIcon, sizeof(NOTIFYICONDATA));
 NotifyIcon.cbSize           = sizeof(NOTIFYICONDATA);
 NotifyIcon.hWnd             = Window;
 NotifyIcon.uFlags           = NIF_ICON | NIF_MESSAGE | NIF_TIP;
 NotifyIcon.hIcon            = Class.hIconSm;
 NotifyIcon.uCallbackMessage = NotifyMessage;
 wcscpy(NotifyIcon.szTip, L"Engineering Calculator");
 Shell_NotifyIcon(NIM_ADD, &NotifyIcon);

 // Create the controls
 Label    = new LABEL    (360, 20, L"=");
 Formula  = new TEXT_BOX (  0, 210);
 FromUnit = new COMBO_BOX(210, 150);
 Result   = new TEXT_BOX (380, 210, true);
 ToUnit   = new COMBO_BOX(590, 150);
 Menu     = new MENU;

 // Get the command-line
 int       ArgC;
 STRING    Arg;
 wchar_t** ArgV = CommandLineToArgvW(GetCommandLineW(), &ArgC);

 // Process the command-line
 if(ArgV){
  for(int j = 1; j < ArgC; j++){
   Arg = ArgV[j];
   if      (!Arg.CompareNoCase("calculator")){
    PostMessage(Window, WM_COMMAND, IDM_CONVERTER, 0);
   }else if(!Arg.CompareNoCase("normal")){
    PostMessage(Window, WM_COMMAND, IDM_NORMAL, 0);
   }else if(!Arg.CompareNoCase("not-on-top")){
    PostMessage(Window, WM_COMMAND, IDM_ALWAYS_ON_TOP, 0);
   }else if(!Arg.CompareNoCase("degrees")){
    PostMessage(Window, WM_COMMAND, IDM_DEGREES, 0);
   }
  }
  LocalFree(ArgV);
 }

 // Show the window
 ShowWindow(Window, SW_SHOW);
 Formula->SetFocus();

 // Load Conversions
 Conversions.Compare = CONVERSION_Compare;
 LoadConversions();

 // Run the message loop
 MSG Message;
 while(GetMessage(&Message, 0, 0, 0)){
  if(!TranslateAccelerator(Window, Menu->AcceleratorTable, &Message)){
   if(!IsDialogMessage(Window, &Message)){
    TranslateMessage(&Message);
    DispatchMessage (&Message);
   }
  }
 }

 // Clean-up
 delete Menu;
 delete Label;
 delete Result;
 delete ToUnit;
 delete Formula;
 delete FromUnit;

 Shell_NotifyIcon(NIM_DELETE, &NotifyIcon);

 DestroyWindow(Window);

 DeleteObject(Font);

 CONVERSION* Conversion = (CONVERSION*)Conversions.First();
 while(Conversion){
  delete Conversion;
  Conversion = (CONVERSION*)Conversions.Next();
 }

 return Message.wParam;
}
//------------------------------------------------------------------------------
