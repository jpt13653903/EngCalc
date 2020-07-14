CC      = g++
Options = -std=c++11 -Wall -fexceptions -O2 -mwindows
#-------------------------------------------------------------------------------

Includes = -I../Source \
           -IControls  \
           -ITools/Cpp \
           -IResources

Libraries  = -lgdi32 -luser32 -lkernel32 -lcomctl32
LibInclude = 
#-------------------------------------------------------------------------------

Version = -DMAJOR_VERSION=1 -DMINOR_VERSION=8

Objects = obj/Global.o                       \
          obj/Controls/ComboBox.o            \
          obj/Controls/Control.o             \
          obj/Controls/Label.o               \
          obj/Controls/Menu.o                \
          obj/Controls/TextBox.o             \
          obj/Tools/Cpp/Calculator.o         \
          obj/Tools/Cpp/FileWrapper.o        \
          obj/Tools/Cpp/General.o            \
          obj/Tools/Cpp/LLRBTree.o           \
          obj/Tools/Cpp/UTF_Converter.o      \
          obj/Tools/Cpp/XML.o                \
          obj/Resources/Resources.res

Headers = *.h           \
          Controls/*.h  \
          Tools/Cpp/*.h \
          Resources/*.h
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

