# The MIT License (MIT)

# Copyright (c) 2014 nabijaczleweli

# Permission is hereby granted, free of charge, to any person obtaining a copy of
# this software and associated documentation files (the "Software"), to deal in
# the Software without restriction, including without limitation the rights to
# use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
# the Software, and to permit persons to whom the Software is furnished to do so,
# subject to the following conditions:

# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
# FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
# COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
# IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
# CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

ifeq ($(OS),Windows_NT)
  MV = move
  EXE = .exe
else
  MV = mv
  EXE =
endif

OBJ = .o
CPP = c++
CPPAR = -s -Os -std=c++11 -Wall -Wextra -pipe -fomit-frame-pointer

.PHONY : clean

all : CPPac-man.o ghost.o levelmaker
	$(CPP) $(CPPAR) -oCPPac-man$(EXE) $(filter %.o,$^)

levelmaker : levels/lvlgen.o
	$(CPP) $(CPPAR) -olevels/lvlgen$(EXE) $(filter %.o,$^)

clean :
	rm -rf *$(OBJ) *$(EXE)
	rm -rf */*$(OBJ) */*$(EXE)

%.o : %.cpp
	$(CPP) $(CPPAR) -c -o$@ $^