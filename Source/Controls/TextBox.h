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

#ifndef TextBox_h
#define TextBox_h
//------------------------------------------------------------------------------

#include "Control.h"
#include "UTF_Converter.h"
//------------------------------------------------------------------------------

class TEXT_BOX: public CONTROL{
  public:
    TEXT_BOX(int Left, int Width, bool ReadOnly = false);
   ~TEXT_BOX();

  void SetText(const char*  String);
  void GetText(std::string* String);
};
//------------------------------------------------------------------------------

#endif
//------------------------------------------------------------------------------
