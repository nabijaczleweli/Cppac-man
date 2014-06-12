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

ifeq "$(OS)" "Windows_NT"
	EXE = .exe
	SYSLDAR = -lkernel32
	nop = echo > nul
else
	EXE = .out
	SYSLDAR = -ldl
	nop = echo > /dev/null
endif

# VERY IMPORTANT NOTE :
#  For me, LD cannot link to -L$(var) -l$(var), nor can it -L. -l$(var), it's just broken.
#  This is all a hack, pretty much.
#  I hope this gets fixed, or maybe that I am doing something wrong...

OBJ = .o
ARCH = .a
AR = ar
CPP = c++
CPPAR = -Os -std=c++11 -Wall -Wextra -pipe -fomit-frame-pointer -shared-libstdc++
# Is "-shared-libstdc++" OK? Does everything support that? Does it make sence?
STRIP = strip
STRIPAR = --strip-all --remove-section=.comment --remove-section=.note
NOLINKSUBPROJS = levels
LINKSUBPROJS = RuntimeDLLs
SUBPROJS = $(NOLINKSUBPROJS) $(LINKSUBPROJS)
LDAR = -lpdcurses
OBJFILESDIR = objects

.PHONY : clean all unpackarchs $(SUBPROJS)

all : $(addsuffix .MAKE.DIR,$(SUBPROJS)) unpackarchs ghost$(OBJ) pacman$(OBJ) utils$(OBJ) CPPac-man$(OBJ)
	$(CPP) $(CPPAR) $(SYSLDAR) $(LDAR) -oCPPac-man$(EXE) $(OBJFILESDIR)/*$(OBJ)
	$(STRIP) $(STRIPAR) CPPac-man$(EXE) -oCPPac-man$(EXE)
	rm -rf $(OBJFILESDIR)

clean :
	$(foreach subproj,$(SUBPROJS),$(MAKE) -C $(subproj) clean &&) $(nop)
	rm -rf *$(OBJ) *$(EXE)

unpackarchs : $(addsuffix .MAKE.DIR,$(LINKSUBPROJS))
ifeq "$(findstring $(OBJFILESDIR),$(basename $(dir $(shell ls))))" ""
	mkdir $(OBJFILESDIR)
endif
	$(foreach subproj,$(^:.MAKE.DIR=),cp $(subproj)/lib$(subproj)$(ARCH) $(OBJFILESDIR)/lib$(shell echo $(subproj) | tr A-Z a-z)$(ARCH) &&) $(nop)
	cd $(OBJFILESDIR) && $(foreach subproj,$(^:.MAKE.DIR=),$(AR) xo lib$(shell echo $(subproj) | tr A-Z a-z)$(ARCH) &&) $(nop)


%.MAKE.DIR :
	$(MAKE) -C $(@:.MAKE.DIR=)

%$(OBJ) : %.cpp
	$(CPP) $(CPPAR) -c -o$(OBJFILESDIR)/$@ $^