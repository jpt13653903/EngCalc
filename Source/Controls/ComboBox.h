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

#ifndef ComboBox_h
#define ComboBox_h
//------------------------------------------------------------------------------

#include "Control.h"
#include "UTF_Converter.h"
//------------------------------------------------------------------------------

class ComboBox: public Control{
    private:
        int itemCount;

    public:
        ComboBox(int left, int width);
       ~ComboBox();

        void clear  ();
        void addItem(const char*  item);
        void getItem(std::string* item);
};
//------------------------------------------------------------------------------

#endif
//------------------------------------------------------------------------------
