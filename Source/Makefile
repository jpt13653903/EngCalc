CC      = g++
Options = -std=c++17 -Wall -fexceptions -O2 -mwindows
#-------------------------------------------------------------------------------

Includes = -I../Source \
           -IControls  \
           -ITools/Cpp \
           -IResources

Libraries  = -lgdi32 -luser32 -lkernel32 -lcomctl32
LibInclude =
#-------------------------------------------------------------------------------

Version = -DMAJOR_VERSION=1 -DMINOR_VERSION=11

Objects += obj/Global.o
Objects += obj/Controls/ComboBox.o
Objects += obj/Controls/Control.o
Objects += obj/Controls/Label.o
Objects += obj/Controls/Menu.o
Objects += obj/Controls/TextBox.o
Objects += obj/Tools/Cpp/Calculator.o
Objects += obj/Tools/Cpp/FileWrapper.o
Objects += obj/Tools/Cpp/General.o
Objects += obj/Tools/Cpp/LLRBTree.o
Objects += obj/Tools/Cpp/UTF_Converter.o
Objects += obj/Tools/Cpp/XML.o
Objects += obj/Resources/Resources.res

Headers += *.h
Headers += Controls/*.h
Headers += Tools/Cpp/*.h
Headers += Resources/*.h
#-------------------------------------------------------------------------------

.PHONY: clean all
.SECONDARY:

all: bin/EngCalc.exe

clean:
	rm -rf bin
	rm -rf obj
#-------------------------------------------------------------------------------

# Binaries

bin/EngCalc.exe: main.cpp $(Objects) $(Headers)
	mkdir -p bin
	$(CC) $(Options) $(Version) $(Includes) $< $(Objects) -s $(LibInclude) $(Libraries) -o $@
#-------------------------------------------------------------------------------

# Objects

obj/%.o: %.cpp $(Headers)
	mkdir -p $(@D)
	$(CC) $(Options) $(Version) $(Defines) -c $(Includes) $< -o $@
#-------------------------------------------------------------------------------

# Resources

obj/%.res: %.rc
	mkdir -p $(@D)
	windres.exe  -J rc -O coff -i $< -o $@
#-------------------------------------------------------------------------------

