QT       += core gui
QT       += multimedia
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

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
    activity.cpp \
    administorwindow.cpp \
    course.cpp \
    images_change.cpp \
    input_activity.cpp \
    input_course.cpp \
    main.cpp \
    mainwindow.cpp \
    page_login.cpp

HEADERS += \
    activity.h \
    administorwindow.h \
    course.h \
    images_change.h \
    input_activity.h \
    input_course.h \
    mainwindow.h \
    page_login.h

FORMS += \
    administorwindow.ui \
    input_activity.ui \
    input_course.ui \
    mainwindow.ui \
    page_login.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
CONFIG+=C++11

RESOURCES += \
    ImageResource.qrc

DISTFILES +=
