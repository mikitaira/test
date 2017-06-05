TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    exec.cpp \
    VoxGraphics.cpp \
    VoxRobot.cpp

LIBS += -lglut -lGLU -lGL



HEADERS += \
    exec.h \
    VoxGraphics.h \
    VoxRobot.h

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../Voxelyze-master/lib/release/ -lvoxelyze
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../Voxelyze-master/lib/debug/ -lvoxelyze
else:unix:!macx: LIBS += -L$$PWD/../Voxelyze-master/lib/ -lvoxelyze

INCLUDEPATH += $$PWD/../Voxelyze-master/include
DEPENDPATH += $$PWD/../Voxelyze-master/include

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../Voxelyze-master/lib/release/libvoxelyze.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../Voxelyze-master/lib/debug/libvoxelyze.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../Voxelyze-master/lib/release/voxelyze.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../Voxelyze-master/lib/debug/voxelyze.lib
else:unix:!macx: PRE_TARGETDEPS += $$PWD/../Voxelyze-master/lib/libvoxelyze.a
