TARGET=PutPixel

CONFIG -=qt
cache()
CONFIG-=app_bundle
DESTDIR=./
SOURCES=BlitTexture.cpp
CONFIG+=c++14

QMAKE_CXXFLAGS+=$$system(sdl2-config  --cflags)
message(output from sdl2-config --cflags added to CXXFLAGS= $$QMAKE_CXXFLAGS)

LIBS+=$$system(sdl2-config  --libs)
message(output from sdl2-config --libs added to LIB=$$LIBS)
