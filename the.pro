QT += core gui widgets multimedia multimediawidgets

CONFIG += c++11


# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        random_time_generator.cpp \
        the_button.cpp \
        the_player.cpp \
        settings.cpp \
        record_video.cpp \
        video_test.cpp \
        tomeo.cpp

HEADERS += \
    random_time_generator.h \
    record_video.h \
    the_button.h \
    settings.h \
    video_test.h \
    the_player.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
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
    icons/record.png \
    icons/flip_light.png \
    icons/flip_dark.png \
    icons/down_arrow.png

