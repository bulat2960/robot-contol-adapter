#-------------------------------------------------
#
# Project created by QtCreator 2018-11-28T21:15:23
#
#-------------------------------------------------

QT       += core network testlib
QT -= gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Test
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11 console
CONFIG -= app_bundle
CONFIG += testcase

SOURCES += \
    Objects/controlunit.cpp \
    Objects/planner.cpp \
    Objects/scene.cpp \
    main.cpp \
    ../System/RobotControlAdapter/robotcontroladapter.cpp \
    ../System/Connectors/controlunitconnector.cpp \
    ../System/Connectors/sceneconnector.cpp \
    ../System/Connectors/plannerconnector.cpp \
    test.cpp

HEADERS += \
    Objects/controlunit.h \
    Objects/planner.h \
    Objects/scene.h \
    ../System/RobotControlAdapter/robotcontroladapter.h \
    ../System/Connectors/controlunitconnector.h \
    ../System/Connectors/sceneconnector.h \
    ../System/Connectors/plannerconnector.h \
    test.h

INCLUDEPATH += ../System/

CONFIG(release, debug|release): DESTDIR = $$OUT_PWD/release
CONFIG(debug, debug|release): DESTDIR = $$OUT_PWD/debug
install_it.path = $$DESTDIR
install_it.files += $$PWD/../config.ini
INSTALLS += install_it

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
