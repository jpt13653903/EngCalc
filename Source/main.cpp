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

using namespace std;
//------------------------------------------------------------------------------

int   monitorCount = 0;
int   monitorIndex = 0;
RECT* monitors;

static BOOL CALLBACK monitorEnum(HMONITOR handle, HDC dc, LPRECT rect, LPARAM data)
{
    if(monitorIndex >= monitorCount) return false;

    monitors[monitorIndex].left   = rect->left;
    monitors[monitorIndex].top    = rect->top;
    monitors[monitorIndex].right  = rect->right;
    monitors[monitorIndex].bottom = rect->bottom;

    monitorIndex++;

    return true;
}
//------------------------------------------------------------------------------

// X and Y are client coordinates of the mouse pointer
static void moveWindow(int x, int y)
{
    // Get the mouse position
    POINT point;
    GetCursorPos(&point);

    // If there are no monitors, something is seriously wrong
    monitorCount = GetSystemMetrics(SM_CMONITORS);
    if(!monitorCount) return;

    // Find the monitor metrics
    monitorIndex = 0;
    monitors     = new RECT[monitorCount];
    EnumDisplayMonitors(0, 0, monitorEnum, 0);

    // Find the screen on which the mouse is
    int j;
    for(j = 0; j < monitorCount-1; j++){
        if(point.x >= monitors[j].left && point.x <= monitors[j].right  &&
              point.y >= monitors[j].top  && point.y <= monitors[j].bottom ){
            break;
        }
    }

    // Make the move
    point.x -= x;
    point.y -= y;

    // Make sure the entire window is on the screen
    if(point.x < monitors[j].left)
       point.x = monitors[j].left;
    if(point.y < monitors[j].top )
       point.y = monitors[j].top ;
    if(point.x > monitors[j].right  - mainWidth)
       point.x = monitors[j].right  - mainWidth;
    if(point.y > monitors[j].bottom - 21)
       point.y = monitors[j].bottom - 21;

    // Apply the new position
    SetWindowPos(window, 0, point.x, point.y, 0, 0, SWP_NOSIZE);

    // Clean up the memory allocation
    delete[] monitors;
}
//------------------------------------------------------------------------------

struct Conversion{
    string fromUnit;
    string toUnit;
    string formula;
};
//------------------------------------------------------------------------------

static int conversionCompare(Conversion* left, Conversion* right)
{
    u32string leftString;
    u32string rightString;

    leftString  = utfConverter.toUpperCase(utfConverter.toUtf32(left ->fromUnit));
    rightString = utfConverter.toUpperCase(utfConverter.toUtf32(right->fromUnit));

    if(leftString < rightString) return -1;
    if(leftString > rightString) return  1;

    if(left->fromUnit < right->fromUnit) return -1;
    if(left->fromUnit > right->fromUnit) return  1;

    leftString  = utfConverter.toUpperCase(utfConverter.toUtf32(left ->toUnit));
    rightString = utfConverter.toUpperCase(utfConverter.toUtf32(right->toUnit));

    if(leftString < rightString) return -1;
    if(leftString > rightString) return  1;

    if(left->toUnit < right->toUnit) return -1;
    if(left->toUnit > right->toUnit) return  1;

    return 0;
}
//------------------------------------------------------------------------------

static int conversionCompareFromUnit(Conversion* left, Conversion* right)
{
    u32string leftString;
    u32string rightString;

    leftString  = utfConverter.toUpperCase(utfConverter.toUtf32(left ->fromUnit));
    rightString = utfConverter.toUpperCase(utfConverter.toUtf32(right->fromUnit));

    if(leftString < rightString) return -1;
    if(leftString > rightString) return  1;

    if(left->fromUnit < right->fromUnit) return -1;
    if(left->fromUnit > right->fromUnit) return  1;

    return 0;
}
//------------------------------------------------------------------------------

static void onFromUnitClick()
{
    Conversion  key;
    Conversion* conversion;
    fromUnitBox->getItem(&key.fromUnit);

    toUnitBox->clear();

    // Finds the first of the list of matches
    conversions.compare = (LLRBTreeCompare)conversionCompareFromUnit;
        conversion = (Conversion*)conversions.find(&key);
    conversions.compare = (LLRBTreeCompare)conversionCompare;

    while(conversion){
        if(key.fromUnit != conversion->fromUnit) break;

        toUnitBox->addItem(conversion->toUnit.c_str());
        conversion = (Conversion*)conversions.next();
    }
}
//------------------------------------------------------------------------------

static void loadConversions()
{
    string      s;
    Conversion* conversion;
    Xml         xml;

    // Read Converter.xml
    if(!xml.load("..\\Converter.xml")) return;

    Xml::Entity* entity;
    entity = xml.findChild(xml.root, "Conversion");
    while(entity){
        conversion = new Conversion;
        xml.readAttribute(entity, "From"   , &conversion->fromUnit);
        xml.readAttribute(entity, "To"     , &conversion->toUnit  );
        xml.readAttribute(entity, "Formula", &conversion->formula );
        if(
            conversion->fromUnit.length() &&
            conversion->toUnit  .length() &&
            conversion->formula .length()
        ){
            conversions.insert(conversion);
        }else{
            delete conversion;
        }
        entity = xml.nextChild(xml.root, "Conversion");
    }

    // Add the conversions to the "fromUnitBox" combo-box.
    s = "";
    conversion = (Conversion*)conversions.first();
    while(conversion){
        if(s != conversion->fromUnit){
            fromUnitBox->addItem(conversion->fromUnit.c_str());
            s = conversion->fromUnit;
        }
        conversion = (Conversion*)conversions.next();
    }

    if(conversions.itemCount()) onFromUnitClick();
}
//------------------------------------------------------------------------------

static bool invalid(long double d, string& s)
{
    unsigned* p;

    // See the "Intel Architecture Software Developer's Manual,
    // Volume 1: Basic Architecture" for details on the floating point format
    p = (unsigned*)&d;
    if(((p[2] & 0x7FFF) == 0x7FFF) && (p[1] & 0x80000000)){
        if(p[2] & 0x8000){
            if(p[1] == 0x80000000) s = "-Infinity";
            else                   s = "Not a number";
        }else{
            s = "Infinite";
        }
        return true;
    }
    return false;
}
//------------------------------------------------------------------------------

static void toHex(long double d, string& s)
{
    int  exp, places;
    char c;
    bool spaces = menu->getGroupDigits();

    if(invalid(d, s)) return;

    if(d == 0.0){
        s = "0";
        return;
    }

    if(d < 0.0){
        s = "-0x";
        d *= -1.0;
    }else{
        s = "0x";
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
            s += menu->getDecimalFormat();
        }else{
            if(spaces && places && ((exp+1) % 4 == 0)) s += ' ';
        }

        c = floorl(d);

        if(c <= 9) s += (char)(c + '0');
        else       s += (char)(c + 'A' - 10);

        d -= c;
        d *= 16.0;

        places++;
        exp   --;
    }
}
//------------------------------------------------------------------------------

static void toBinary(long double d, string& s)
{
    int  exp, places;
    char c;
    bool spaces = menu->getGroupDigits();

    if(invalid(d, s)) return;

    if(d == 0.0){
        s = "0";
        return;
    }

    if(d < 0.0){
        s = "-0b";
        d *= -1.0;
    }else{
        s = "0b";
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
            s += menu->getDecimalFormat();
        }else{
            if(spaces && places && ((exp+1) % 4 == 0)) s += ' ';
        }

        c = floorl(d);

        s += (char)(c + '0');

        d -= c;
        d *= 2.0;

        places++;
        exp   --;
    }
}
//------------------------------------------------------------------------------

static void toDecimal(long double d, string& s)
{
    int  exp, exponent;
    bool spaces = menu->getGroupDigits();

    if(invalid(d, s)) return;

    if(d == 0.0){
        s = "0";
        return;
    }

    s = "";
    bool sign = false;
    if(d < 0.0){
        d *= -1.0;
        sign = true;
    }

    exponent = 0;
    switch(menu->getDisplayMode()){
        case Menu::DisplayMode::Normal:
            if(d > 1e6){
                while(d >= 10.0){
                    d /= 10.0;
                    exponent++;
                }
            }else if(d < 1e-6){
                while(d < 1.0){
                    d *= 10.0;
                    exponent--;
                }
            }
            break;

        case Menu::DisplayMode::Engineering:
            while(d >= 1e3){
                d /= 1e3;
                exponent += 3;
            }
            while(d < 1.0){
                d *= 1e3;
                exponent -= 3;
            }
            break;

        default: // Feet & Inches or Deg, Min & Sec
            break;
    }

    exp = 0;
    while(d >= 10.0){
        d  /= 10.0;
        exp++;
    }

    d    = roundl(d*powl(10.0, digits-1));
    exp -= digits-1;

    // Add trailing zeroes
    for(int n = 0; n < exp; n++){
        if(spaces && n % 3 == 0) s += ' ';
        s += '0';
    }

    // Remove zeros after decimal point
    while(exp < 0){
        if(fmodl(d, 10.0) != 0.0) break;
        d = floorl(d / 10.0);
        exp++;
    }

    while(d > 0.0 || exp <= 0){
        if     (exp     == 0 && s.length()) s += menu->getDecimalFormat();
        else if(exp % 3 == 0 && spaces    ) s += ' ';

        s += (char)(fmodl(d, 10.0) + '0');
        d = floorl(d / 10.0);
        exp++;
    }

    if(sign) s += '-';

    // Reverse
    u32string data_32 = utfConverter.toUtf32(s);
    int     length_32 = data_32.length();
    int l = length_32 / 2;
    char32_t c;

    for(int n = 0; n < l; n++){
        c = data_32[n];
        data_32[n] = data_32[length_32-n-1];
        data_32[length_32-n-1] = c;
    }
    s = utfConverter.toUtf8(data_32);

    if(exponent){
        if(spaces) s += ' ';
        s += 'e';
        s += to_string(exponent);
    }
}
//------------------------------------------------------------------------------

static void getString(long double d, string& s)
{
    switch(menu->getFormat()){
        case Menu::Format::Binary:
            toBinary(d, s);
            break;

        case Menu::Format::Hexadecimal:
            toHex(d, s);
            break;

        default:
            toDecimal(d, s);
            break;
    }
    // Remove trailing spaces...
    while(s.length() && s[s.length()-1] == ' ') s.resize(s.length()-1);
}
//------------------------------------------------------------------------------

static void calculate()
{
    string s, formula;

    formulaBox->getText(&formula);
    if(formula.empty()){
        resultBox->setText("");
        return;
    }

    long double result = calc.calculate(formula.c_str());
    if(menu->getConverter()){
        Conversion  key;
        Conversion* conversion;
        fromUnitBox->getItem(&key.fromUnit);
        toUnitBox  ->getItem(&key.toUnit  );

        conversion = (Conversion*)conversions.find(&key);
        if(!conversion) result = 0.0;
        else result = calc.calculate(conversion->formula.c_str(), "x", result);
    }

    switch(menu->getDisplayMode()){
        case Menu::DisplayMode::FeetInches:{
            long double i, f; // integer and fraction parts
            string temp;
            f = modfl(result, &i);
            getString(i, temp);
            s += temp + "' ";
            getString(12*f, temp);
            s += temp + '"';
            break;
        }

        case Menu::DisplayMode::DegMinSec:{
            long double i, f; // integer and fraction parts
            string temp;
            f = modfl(result, &i);
            getString(i, temp);
            s += temp + "° ";
            f = modfl(60*f, &i);
            getString(i, temp);
            s += temp + "' ";
            getString(60*f, temp);
            s += temp + '"';
            break;
        }

        default:
            getString(result, s);
    }
    resultBox->setText(s.c_str());
}
//------------------------------------------------------------------------------

static void onAboutClick()
{
    wchar_t message[0x1000];
    wsprintf(message,
        L"Engineering Calculator, Version %d.%d\n"
        L"Built on " __DATE__ " at " __TIME__ "\n"
        L"\n"
        L"Copyright (C) John-Philip Taylor\n"
        L"jpt13653903@gmail.com\n"
        L"\n"
        L"Developed using:\n"
        L"- MinGW (tdm64-1) 10.3.0 <http://tdm-gcc.tdragon.net/>\n"
        L"\n"
        L"This program is free software: you can redistribute it\n"
        L"and/or modify it under the terms of the GNU General\n"
        L"Public License as published by the Free Software\n"
        L"Foundation, either version 3 of the License, or\n"
        L"(at your option) any later version.\n"
        L"\n"
        L"This program is distributed in the hope that it will\n"
        L"be useful, but WITHOUT ANY WARRANTY; without even the\n"
        L"implied warranty of MERCHANTABILITY or FITNESS FOR A\n"
        L"PARTICULAR PURPOSE. See the GNU General Public License\n"
        L"for more details.\n"
        L"\n"
        L"You should have received a copy of the GNU General\n"
        L"Public License along with this program.  If not,\n"
        L"see <http://www.gnu.org/licenses/>",
        MAJOR_VERSION, MINOR_VERSION
    );
    MessageBox(window, message, L"About", MB_ICONINFORMATION);
}
//------------------------------------------------------------------------------

static LRESULT CALLBACK windowProcedure(
    HWND   handle,
    UINT   message,
    WPARAM wParam,
    LPARAM lParam
){
    static int x, y;

    RECT   rect;
    string s;

    switch(message){
        case WM_CLOSE:
        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        case WM_LBUTTONDOWN:
            x = GET_X_LPARAM(lParam);
            y = GET_Y_LPARAM(lParam);
            SetCapture(window);
            break;

        case WM_MOUSEMOVE:
            if(wParam & MK_LBUTTON) moveWindow(x, y);
            break;

        case WM_LBUTTONUP:
            ReleaseCapture();
            break;

        case WM_RBUTTONUP:
            menu->show();
            break;

        case NotifyMessage:
            switch(lParam){
                case WM_LBUTTONDOWN:
                    ShowWindow         (window, SW_SHOW);    // Show if hidden
                    SetForegroundWindow(window);             // Activate if in the background
                    SetFocus           (formulaBox->handle); // Set user input to the formula
                    break;

                case WM_RBUTTONUP:
                    menu->show();
                    break;

                default:
                    break;
            }
            break;

        case WM_COMMAND:
            if((HWND)lParam == formulaBox->handle && HIWORD(wParam) == EN_CHANGE){
                calculate();

            }else if(
                (HWND) lParam  == fromUnitBox->handle &&
                HIWORD(wParam) == CBN_SELCHANGE
            ){
                onFromUnitClick();
                calculate();

            }else if(
                (HWND) lParam  == toUnitBox->handle &&
                HIWORD(wParam) == CBN_SELCHANGE
            ){
                calculate();

            }else if(lParam == 0){
                switch(LOWORD(wParam)){
                    case IDM_DEGREES:
                        menu->setRadians(false);
                        calc.measure = Calculator::Degrees;
                        calculate();
                        break;

                    case IDM_RADIANS:
                        menu->setRadians(true);
                        calc.measure = Calculator::Radians;
                        calculate();
                        break;

                    case IDM_DECIMAL:
                        menu->setFormat(Menu::Format::Decimal);
                        calculate();
                        break;

                    case IDM_HEX:
                        menu->setFormat(Menu::Format::Hexadecimal);
                        calculate();
                        break;

                    case IDM_BINARY:
                        menu->setFormat(Menu::Format::Binary);
                        calculate();
                        break;

                    case IDM_NORMAL:
                        menu->setDisplayMode(Menu::DisplayMode::Normal);
                        calculate();
                        break;

                    case IDM_ENGINEERING:
                        menu->setDisplayMode(Menu::DisplayMode::Engineering);
                        calculate();
                        break;

                    case IDM_FEET_INCHES:
                        menu->setDisplayMode(Menu::DisplayMode::FeetInches);
                        calculate();
                        break;

                    case IDM_DEG_MIN_SEC:
                        menu->setDisplayMode(Menu::DisplayMode::DegMinSec);
                        calculate();
                        break;

                    case IDM_GROUP_DIGITS:
                        menu->setGroupDigits(!menu->getGroupDigits());
                        calculate();
                        break;

                    case IDM_INCREASE_DIGITS:
                        if(digits < 18) digits++;
                        calculate();
                        break;

                    case IDM_DECREASE_DIGITS:
                        if(digits > 3) digits--;
                        calculate();
                        break;

                    case IDM_DOT_DECIMALS:
                        menu->setDecimalFormat('.');
                        calculate();
                        break;

                    case IDM_COMMA_DECIMALS:
                        menu->setDecimalFormat(',');
                        calculate();
                        break;

                    case IDM_ALWAYS_ON_TOP:
                        LONG style;
                        style = GetWindowLong(window, GWL_EXSTYLE);
                        if(menu->getAlwaysOnTop()){
                            style &= ~WS_EX_TOPMOST;
                            menu->setAlwaysOnTop(false);
                            SetWindowLong(window, GWL_EXSTYLE, style);
                            SetWindowPos(
                                window,
                                HWND_NOTOPMOST,
                                0, 0, 0, 0,
                                SWP_NOMOVE | SWP_NOSIZE
                            );
                        }else{
                            style |= WS_EX_TOPMOST;
                            menu->setAlwaysOnTop(true);
                            SetWindowLong(window, GWL_EXSTYLE, style);
                            SetWindowPos(
                                window,
                                HWND_TOPMOST,
                                0, 0, 0, 0,
                                SWP_NOMOVE | SWP_NOSIZE
                            );
                        }
                        break;

                    case IDM_CONVERTER:
                        GetWindowRect(window, &rect);
                        if(menu->getConverter()){
                            mainWidth  = 440;
                            rect.left += 300;

                            label      ->setLeft(210);
                            resultBox  ->setLeft(230);
                            fromUnitBox->hide();
                            toUnitBox  ->hide();
                            menu       ->setConverter(false);

                        }else{
                            mainWidth  = 740;
                            rect.left -= 300;

                            fromUnitBox->show();
                            toUnitBox  ->show();
                            label      ->setLeft(360);
                            resultBox  ->setLeft(380);
                            menu       ->setConverter(true);
                        }
                        SetWindowPos(window, 0, rect.left, rect.top, mainWidth, 21, 0);
                        resultBox ->getText(&s);
                        formulaBox->setText( s.c_str());
                        calculate();
                        break;

                    case IDM_MINIMIZE:
                        ShowWindow(window, SW_HIDE);
                        break;

                    case IDM_EXIT:
                        PostQuitMessage(0);
                        break;

                    case IDM_MANUAL:
                        ShellExecute(
                            0, L"open",
                            L"\"https://sourceforge.net/p/alwaysontopcalc/wiki/Home/\"",
                            0, 0, SW_MAXIMIZE
                        );
                        break;

                    case IDM_ABOUT:
                        onAboutClick();
                        break;

                    default:
                        break;
                }
            }
            break;

        default:
            return DefWindowProc(handle, message, wParam, lParam);
    }
    return 0;
}
//------------------------------------------------------------------------------

static HICON loadIconSmall(WORD icon)
{
    int size = GetSystemMetrics(SM_CXSMICON);

    return (HICON)LoadImage(
        instance,
        MAKEINTRESOURCE(icon),
        IMAGE_ICON,
        size, size,
        LR_SHARED
    );
}
//------------------------------------------------------------------------------

static void complexArgument(string& arg)
{
    // Arg is already in upper-case

    if(!arg.compare(0, 11, "RIGHTSPACE=")){
        rightSpace = 0;
        for(int n = 11; arg[n] >= '0' && arg[n] <= '9'; n++){
            rightSpace = 10*rightSpace + arg[n]-'0';
        }

    }else if(!arg.compare(0, 7, "DIGITS=")){
        digits = 0;
        for(int n = 7; arg[n] >= '0' && arg[n] <= '9'; n++){
            digits = 10*digits + arg[n]-'0';
        }
        if(digits <  3) digits =  3;
        if(digits > 18) digits = 18;
    }
}
//------------------------------------------------------------------------------

int WINAPI WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR     commandLine,
    int       cmdShow
){
    INITCOMMONCONTROLSEX icex;
    icex.dwSize = sizeof(icex);
    icex.dwICC  = 0xFFFF; // Load everything
    InitCommonControlsEx(&icex);

    // Make sure the "working folder" is correct
    int            j, q, i;
    const wchar_t* fullCommandLine = GetCommandLineW();
          wchar_t* newCurrentDirectory;

    for(j = 0; fullCommandLine[j]; j++);
    while(j && fullCommandLine[j] != '\\') j--;
    if(j){
        newCurrentDirectory = new wchar_t[j+2];
        q = i = 0;
        if(fullCommandLine[q] == '"') q++;
        while(q <= j) newCurrentDirectory[i++] = fullCommandLine[q++];
        newCurrentDirectory[i] = 0;

        SetCurrentDirectoryW(newCurrentDirectory);
        delete[] newCurrentDirectory;
    }

    // Get the command-line
    int       argC;
    string    arg;
    wchar_t** argV = CommandLineToArgvW(fullCommandLine, &argC);

    bool calculator    = false;
    bool normal        = false;
    bool notOnTop      = false;
    bool degrees       = false;
    bool commaDecimals = false;

    // Process the command-line
    if(argV){
        for(int n = 1; n < argC; n++){
            arg = utfConverter.toUpperCase(utfConverter.toUtf8((char16_t*)(argV[n])));
            if     (arg == "CALCULATOR"   ) calculator    = true;
            else if(arg == "NORMAL"       ) normal        = true;
            else if(arg == "NOT-ON-TOP"   ) notOnTop      = true;
            else if(arg == "DEGREES"      ) degrees       = true;
            else if(arg == "COMMADECIMALS") commaDecimals = true;
            else complexArgument(arg);
        }
        LocalFree(argV);
    }

    // Store the Instance handle in the global variable
    instance = hInstance;

    // Register the Window Class
    WNDCLASSEX windowClass;
    windowClass.cbSize        = sizeof(WNDCLASSEX);
    windowClass.hInstance     = instance;
    windowClass.lpszClassName = L"EngineeringCalculator";
    windowClass.lpfnWndProc   = windowProcedure;
    windowClass.style         = 0;
    windowClass.hIcon         = LoadIcon(instance, MAKEINTRESOURCE(MainIcon));
    windowClass.hIconSm       = loadIconSmall(MainIcon);
    windowClass.hCursor       = LoadCursor(0, IDC_ARROW);
    windowClass.lpszMenuName  = 0;
    windowClass.cbClsExtra    = 0;
    windowClass.cbWndExtra    = 0;
    windowClass.hbrBackground = (HBRUSH)COLOR_WINDOW;

    if(!RegisterClassEx(&windowClass)) return 2;

    // Get the system font
    NONCLIENTMETRICS metrics;
    metrics.cbSize = sizeof(NONCLIENTMETRICS);
    SystemParametersInfo(SPI_GETNONCLIENTMETRICS, metrics.cbSize, &metrics, 0);
    font = CreateFontIndirect(&metrics.lfCaptionFont);

    // Create the window
    RECT desktop;
    GetWindowRect(GetDesktopWindow(), &desktop);

    mainWidth = 740;

    window = CreateWindowEx(
        WS_EX_TOPMOST | WS_EX_TOOLWINDOW,
        L"EngineeringCalculator",
        L"Eng Calc",
        WS_POPUP,
        desktop.right - mainWidth - rightSpace, 0,
        mainWidth, 21,
        HWND_DESKTOP,
        0,
        instance,
        0
    );
    SendMessage(window, WM_SETFONT, (WPARAM)font, 0);

    // Add a notification icon
    NOTIFYICONDATA notifyIcon;
    ZeroMemory(&notifyIcon, sizeof(NOTIFYICONDATA));
    notifyIcon.cbSize           = sizeof(NOTIFYICONDATA);
    notifyIcon.hWnd             = window;
    notifyIcon.uFlags           = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    notifyIcon.hIcon            = windowClass.hIconSm;
    notifyIcon.uCallbackMessage = NotifyMessage;
    wcscpy(notifyIcon.szTip, L"Engineering Calculator");
    Shell_NotifyIcon(NIM_ADD, &notifyIcon);

    // Create the controls
    label       = new Label   (360, 20, L"=");
    formulaBox  = new TextBox (  0, 210);
    fromUnitBox = new ComboBox(210, 150);
    resultBox   = new TextBox (380, 210, true);
    toUnitBox   = new ComboBox(590, 150);
    menu        = new Menu;

    if(calculator   ) PostMessage(window, WM_COMMAND, IDM_CONVERTER     , 0);
    if(normal       ) PostMessage(window, WM_COMMAND, IDM_NORMAL        , 0);
    if(notOnTop     ) PostMessage(window, WM_COMMAND, IDM_ALWAYS_ON_TOP , 0);
    if(degrees      ) PostMessage(window, WM_COMMAND, IDM_DEGREES       , 0);
    if(commaDecimals) PostMessage(window, WM_COMMAND, IDM_COMMA_DECIMALS, 0);

    // Show the window
    ShowWindow(window, SW_SHOW);
    formulaBox->setFocus();

    // Load Conversions
    conversions.compare = (LLRBTreeCompare)conversionCompare;
    loadConversions();

    // Run the message loop
    MSG message;
    while(GetMessage(&message, 0, 0, 0)){
        if(!TranslateAccelerator(window, menu->acceleratorTable, &message)){
            if(!IsDialogMessage(window, &message)){
                TranslateMessage(&message);
                DispatchMessage (&message);
            }
        }
    }

    // Clean-up
    delete menu;
    delete label;
    delete resultBox;
    delete toUnitBox;
    delete formulaBox;
    delete fromUnitBox;

    Shell_NotifyIcon(NIM_DELETE, &notifyIcon);

    DestroyWindow(window);

    DeleteObject(font);

    Conversion* conversion = (Conversion*)conversions.first();
    while(conversion){
        delete conversion;
        conversion = (Conversion*)conversions.next();
    }

    return message.wParam;
}
//------------------------------------------------------------------------------
