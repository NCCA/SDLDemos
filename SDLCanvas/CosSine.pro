CONFIG += console
CONFIG -= app_bundle
TARGET = CosSine
INCLUDEPATH+=include

SOURCES+=main.cpp \
         SDLCanvas.cpp

HEADERS += SDLCanvas.h


LIBS+=$$system(sdl-config  --libs)
LIBS+=-L/usr/local/lib -lSDL_ttf
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

