CONFIG += console
CONFIG -= app_bundle

INCLUDEPATH+=include

SOURCES+= src/main.cpp \
    src/SDLOpenGL.cpp \
    src/Rasterizer.cpp \
    src/PixelBuffer.cpp

HEADERS += \
    include/StaticArray.hpp \
    include/Rasterizer.hpp \
    include/PixelBuffer.hpp



LIBS+=$$system(sdl-config  --libs)
message(output from sdl-config --libs added to LIBS =$$LIBS)
CXX_FLAGS+=$$system(sdl-config  --cflags)
message(output from sdl-config --cflags added to CXX_FLAGS= $$CXX_FLAGS)
LIBS+=-lGL
LIBS += -L/usr/lib64

linux-g++*:{
          DEFINES += LINUX
          LIBS+=-lGLEW
}
macx:DEFINES += DARWIN
macx:LIBS+= -L/System/Library/Frameworks/OpenGL.framework/Libraries -framework OpenGL

