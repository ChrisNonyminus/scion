TARGET_GAME ?= sims2

SHELL := bash

CC := i686-linux-gnu-g++-12
LD := i686-linux-gnu-ld

FRAMEWORK_SRC = framework/src
FRAMEWORK_INC = framework/include

GAME_SRC = app/${TARGET_GAME}/src
GAME_INC = app/${TARGET_GAME}/src

PLATFORM_SRC = src
PLATFORM_INC = src

ALL_SOURCES =
ALL_SOURCES += $(wildcard $(GAME_SRC)/*.cpp)
#ALL_SOURCES += $(wildcard $(FRAMEWORK_SRC)/*.cpp)

# source files I will just add manually for now
ALL_SOURCES += $(FRAMEWORK_SRC)/RZStatics.cpp
ALL_SOURCES += $(FRAMEWORK_SRC)/cRZString.cpp
ALL_SOURCES += $(FRAMEWORK_SRC)/cRZCallbackTimer.cpp
ALL_SOURCES += $(FRAMEWORK_SRC)/cRZVersion.cpp
ALL_SOURCES += $(FRAMEWORK_SRC)/cRZDirectory.cpp

ALL_SOURCES += $(wildcard $(PLATFORM_SRC)/*.cpp)
ALL_SOURCES += $(wildcard $(PLATFORM_SRC)/Win32API/*.cpp)
ALL_SOURCES += $(wildcard $(PLATFORM_SRC)/Win32API/DirectX/*.cpp)
#ALL_SOURCES += macho-loader/mach-o.cc
#ALL_SOURCES += macho-loader/ld-mac.cc
ALL_SOURCES += ${TARGET_GAME}.cpp
# get the object files from the source files
ALL_OBJECTS := $(patsubst %.cpp,build/%.o,$(ALL_SOURCES))
ALL_OBJECTS += build/macho-loader/mach-o.o
ALL_OBJECTS += build/macho-loader/log.o
ALL_OBJECTS += build/macho-loader/fat.o
#ALL_OBJECTS += build/macho-loader/ld-mac.o

.PHONY: dxvk-native openssl-0.9.7 mac-headers

# For each framework in the src/MacFrameworks/MacOSX10.4u.sdk/System/Library/Frameworks folder (recursive, folder name ends with .framework), make a symlink in build/src/MacFrameworks that points to the headers folder, but named after the framework name (without the .framework extension), if it doesn't already exist
# This is so that the gcc linux compiler can find the headers
mac-headers:
	mkdir -p build/src/MacFrameworks
	for framework in `find ${CURDIR}/src/MacFrameworks/MacOSX10.4u.sdk/System/Library/Frameworks -name "*.framework" -type d`; do \
		framework_name=`basename $$framework .framework`; \
		if [ ! -d build/src/MacFrameworks/$$framework_name ]; then \
			ln -s $$framework/Versions/A/Headers/ build/src/MacFrameworks/$$framework_name; \
		fi; \
	done


CFLAGS := -I$(GAME_INC) -I$(FRAMEWORK_INC) -I$(PLATFORM_INC) -O0 -g -std=c++03  -DTARGET_GAME_${TARGET_GAME} -Wall
MAC_CFLAGS :=  -Wno-multichar -Isrc/MacFrameworks/MacOSX10.4u.sdk/Developer/Headers/CFMCarbon -Ibuild/src/MacFrameworks -Ibuild/src -Imacho-loader/include -Imacho-loader

CXXFLAGS = $(CFLAGS)
CXXFLAGS += $(MAC_CFLAGS)  -Wno-unknown-pragmas -Idxvk-native/include -Idxvk-native/include -Idxvk-native/include/native  -Idxvk-native/include/native/windows  -Idxvk-native/include/native/directx -Ithird_party/openssl-0.9.7/include
CXXFLAGS += -D_GLIBCXX_USE_CXX11_ABI=0 -fabi-version=2
CXXFLAGS += -ggdb -fsanitize=address -fno-omit-frame-pointer


all: $(TARGET_GAME)
build/macho-loader/libmac/%.o: macho-loader/libmac/%.c
	ccache i686-linux-gnu-gcc-12 -Imacho-loader/include -m32 $(CFLAGS) -c $< -o $@

libmac.so: build/macho-loader/libmac/mac.o build/macho-loader/libmac/strmode.o
	i686-linux-gnu-gcc-12 -m32 -shared $^ -O -fPIC -o $@ -luuid -lz
clean:
	rm -rf build
	rm -f $(TARGET_GAME)
	#rm -rf dxvk-native-master
	#rm -rf third_party/openssl-1.1.1s/build

build:
	mkdir -p build
	mkdir -p build/app/${TARGET_GAME}/src
	mkdir -p build/framework/src
	mkdir -p build/src
	mkdir -p build/src/Win32API
	mkdir -p build/src/Win32API/DirectX
	mkdir -p build/macho-loader
	mkdir -p build/macho-loader/libmac

dxvk-native:
ifeq ($(wildcard ./dxvk-native-master/.),)
	$(shell CC=i686-linux-gnu-gcc-12 CXX=i686-linux-gnu-g++-12  ./dxvk/package-native.sh master ./ --no-package --dev-build)
endif

openssl-0.9.7:
	cd third_party/openssl-0.9.7 && $(MAKE)

build/%.o: %.cpp mac-headers
	ccache $(CC) -m32 $(CXXFLAGS) -c $< -o $@


build/%.o: %.cc
	ccache $(CC) -m32 $(CXXFLAGS) -c $< -o $@

check-env:
ifndef TARGET_GAME
	$(error TARGET_GAME is undefined)
endif


$(TARGET_GAME): check-env build dxvk-native openssl-0.9.7 $(ALL_OBJECTS) libmac.so
	cp dxvk-native-master/usr/lib32/libdxvk_d3d9.so .
	$(CC) -m32 -g -o $@ -T $(TARGET_GAME).ld -Wl,-Map,build/$@.map -fabi-version=2 $(ALL_OBJECTS) -std=c++98 -l:libboost_system.a -l:libboost_filesystem.a -L/usr/local/i386-linux-gnu/lib -llua -llualib `sdl2-config --libs` -lGL -ldl   -L. -ldxvk_d3d9 -l:third_party/openssl-0.9.7/libssl.a -l:third_party/openssl-0.9.7/libcrypto.a -lvulkan  -ggdb -fsanitize=address -fno-omit-frame-pointer -static-libasan -lrt



