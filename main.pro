TEMPLATE = app
TARGET = mp3_player

QT = core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QT += multimedia

SOURCES += \
    src/main.cpp \
    src/Mp3Player.cpp \
    src/MusicApp.cpp \
    src/equalizer.cpp

HEADERS += \
   include/Mp3Player.h \
   include/MusicApp.h \
   include/equalizer.h

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

