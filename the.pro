QT += core gui widgets multimedia multimediawidgets \
    quick

CONFIG += c++11


# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp \
        record_video.cpp \
        settings.cpp \
        the_button.cpp \
        the_player.cpp \
        tomeo.cpp \
        video_test.cpp

HEADERS += \
    record_video.h \
    settings.h \
    the_button.h \
    the_player.h \
    video_test.h

QMAKE_INFO_PLIST = Info.plist

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    Info.plist \
    videos/a.MOV \
    videos/a.png \
    videos/a.wmv \
    videos/b.MOV \
    videos/b.png \
    videos/b.wmv \
    videos/c.mp4 \
    videos/c.png \
    videos/c.wmv \
    videos/d.MOV \
    videos/d.png \
    videos/d.wmv \
    videos/e.mp4 \
    videos/e.png \
    videos/e.wmv \
    videos/f.mp4 \
    videos/f.png \
    videos/f.wmv \
    videos/g.mp4 \
    videos/g.png \
    videos/g.wmv \
    icons/flip_dark.png \
    icons/flip_light.png \
    icons/record.png

