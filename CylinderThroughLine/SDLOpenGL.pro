isEqual(QT_MAJOR_VERSION, 5) {
		cache()
		DEFINES +=QT5BUILD
}
QT += core
QT +=opengl
TARGET=SDLOpenGL
CONFIG-=app_bundle
DESTDIR=./
OBJECTS_DIR=obj
SOURCES=main.cpp

# add the ngl lib
LIBS +=  -L/$(HOME)/NGL/lib -l NGL
# this is where to look for includes
INCLUDEPATH += $$(HOME)/NGL/include/
INCLUDEPATH +=include
HEADERS+=
# add the ngl lib
# this is where to look for includes
INCLUDEPATH +=include

QMAKE_CXXFLAGS+= -msse -msse2 -msse3
macx:QMAKE_CXXFLAGS+= -arch x86_64
linux-g++:QMAKE_CXXFLAGS +=  -march=native
linux-g++-64:QMAKE_CXXFLAGS +=  -march=native

QMAKE_CXXFLAGS+=$$system(/usr/local/bin/sdl-config  --cflags)
message(output from sdl-config --cflags added to CXXFLAGS= $$QMAKE_CXXFLAGS)

LIBS+=$$system(/usr/local/bin/sdl-config  --libs)
message(output from sdl-config --libs added to LIB=$$LIBS)



LIBS += -L/usr/local/lib
macx:LIBS+= -framework OpenGL
macx:DEFINES+=DARWIN
# now if we are under unix and not on a Mac (i.e. linux) define GLEW
linux-g++:linux-g++-64 {
    DEFINES += LINUX
    LIBS+= -lGLEW
}
linux-clang {
    DEFINES += LINUX
    LIBS+= -lGLEW
}
