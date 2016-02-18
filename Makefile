# Makefile for Irrlicht Examples
# It's usually sufficient to change just the target name and source file list
# and be sure that CXX is set to a valid compiler

# For future reference, g++ xmlread.cpp -ltinyxml works.

Target = KhanDema
Sources = main.cpp

# general compiler settings
CPPFLAGS = -I/usr/include/irrlicht -I/usr/local/include/cAudio
MINGWINCLUDES = -IC:\Irrlicht\irrlicht-1.7.1\include
MINGWLIBS = -LC:\Irrlicht\irrlicht-1.7.1\lib\Win32-gcc -lIrrlicht -enable-auto-import

CXXFLAGS = -O3 -ffast-math
#CXXFLAGS = -g -Wall

#default target is Linux

all: all_linux

run: all
	./KhanDema 800 600 w

xml:
	$(warning Building...)
	$(CXX) -ltinyxml xmlread.cpp -o XMLRead 

caudio:
	if [ ! -f /usr/local/lib/cAudio2.1.0/libcAudio.2.1.0.so ]; \
	then \
		make -C dependencies/cAudio/Source ; \
		sudo make -C dependencies/cAudio/Source install; \
	else \
		echo "cAudio found." ; \
	fi \

xmlrun: xml
	./XMLRead
	
mingw32:
	$(warning Building...)
	$(CXX) $(MINGWINCLUDES) $(CXXFLAGS) $(Sources) -o $(DESTPATH) $(MINGWLIBS)

ifeq ($(HOSTTYPE), x86_64)
LIBSELECT=64
endif

# target specific settings
all_linux: LDFLAGS = -L/usr/lib$(LIBSELECT) -lIrrlicht -lGL -lXxf86vm -lXext -lX11 -ltinyxml -LcAudio/lib/linux-x86 -lcAudio
collisiontest: LDFLAGS = -L/usr/lib$(LIBSELECT) -lIrrlicht -lGL -lXxf86vm -lXext -lX11
all_linux clean_linux: SYSTEM=Linux
all_win32: LDFLAGS = -L../../lib/Win32-gcc -lIrrlicht -lopengl32 -lm
all_win32: CPPFLAGS += -D__GNUWIN32__ -D_WIN32 -DWIN32 -D_WINDOWS -D_MBCS -D_USRDLL
all_win32 clean_win32: SYSTEM=Win32-gcc
all_win32 clean_win32: SUF=.exe
# name of the binary - only valid for targets which set SYSTEM
DESTPATH = $(Target)$(SUF)

all_linux all_win32: caudio
	$(warning Building...)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(Sources) -o $(DESTPATH) $(LDFLAGS)

collisiontest:
	$(warning Building...)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) collisiontest.cpp -o CollisionDema $(LDFLAGS)

clean: clean_linux clean_win32
	$(warning Cleaning...)

clean_linux clean_win32:
	@$(RM) $(DESTPATH)

.PHONY: all all_win32 clean clean_linux clean_win32
