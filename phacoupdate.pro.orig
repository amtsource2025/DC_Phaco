QT       += core gui sql


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11 release precompile_header
TARGET = iNanoplusVp_ver1.3
QMAKE_CXXFLAGS_RELEASE += -O3 -g0 -DNDEBUG

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    doctor.cpp \
    footpedal.cpp \
    footswitch.cpp \
    keypad.cpp \
    ltc2614.cpp \
    main.cpp \
    mainwindow.cpp \
    prime.cpp \
    tuning.cpp

HEADERS += \
    doctor.h \
    footlib.h \
    footpedal.h \
    footswitch.h \
    hwhandler.h \
    keypad.h \
    ltc2614.h \
    mainwindow.h \
    prime.h \
    tuning.h \
    vaccum.h

FORMS += \
    doctor.ui \
    footpedal.ui \
    footswitch.ui \
    keypad.ui \
    mainwindow.ui \
    prime.ui \
    tuning.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    img.qrc



win32:CONFIG(release, debug|release): LIBS += -L$$PWD/./release/ -lfootlib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/./debug/ -lfootlib
else:unix: LIBS += -L$$PWD/./ -lfootlib

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/./release/libfootlib.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/./debug/libfootlib.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/./release/footlib.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/./debug/footlib.lib
else:unix: PRE_TARGETDEPS += $$PWD/./libfootlib.a

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/./release/ -lhwhandler
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/./debug/ -lhwhandler
else:unix: LIBS += -L$$PWD/./ -lhwhandler

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/./release/libhwhandler.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/./debug/libhwhandler.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/./release/hwhandler.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/./debug/hwhandler.lib
else:unix: PRE_TARGETDEPS += $$PWD/./libhwhandler.a

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/./release/ -lvaccum
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/./debug/ -lvaccum
else:unix: LIBS += -L$$PWD/./ -lvaccum

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/./release/libvaccum.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/./debug/libvaccum.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/./release/vaccum.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/./debug/vaccum.lib
else:unix: PRE_TARGETDEPS += $$PWD/./libvaccum.a

DISTFILES +=
