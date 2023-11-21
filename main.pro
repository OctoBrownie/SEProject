TEMPLATE = app
TARGET = mp3_player

QT = core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QT += multimedia

SOURCES += \
    src/main.cpp \
    src/mp3player.cpp \
    src/app.cpp \
    src/equalizer.cpp \
    src/song.cpp \
    src/customsongwidget.cpp

HEADERS += \
   include/mp3player.h \
   include/app.h \
   include/equalizer.h \
   include/customsongwidget.h \
   include/song.h

RESOURCES += \
    resources.qrc

INCLUDEPATH += include/

# for fftw3
INCLUDEPATH += $$PWD/lib/fftw-3.3.5-dll64
LIBS += -L$$PWD/lib/fftw-3.3.5-dll64 -llibfftw3-3

# for ffmpeg
INCLUDEPATH += $$PWD/lib/ffmpeg-6.1/include
LIBS += -L$$PWD/lib/ffmpeg-6.1/lib -L$$PWD/lib/ffmpeg-6.1/bin -lavcodec -lavformat -lavutil

# for SDL
INCLUDEPATH += $$PWD/lib/SDL2-2.28.5/include
LIBS += -L$$PWD/lib/SDL2-2.28.5/lib -lsdl2

# for TagLib
INCLUDEPATH += $$PWD/taglib/include
LIBS += -L$$PWD/taglib/lib/ -ltag

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/taglib/lib/tag.lib
else:win32-g++: PRE_TARGETDEPS += $$PWD/taglib/lib/libtag.a
DEFINES += TAGLIB_STATIC
