#-------------------------------------------------
#
# Project created by QtCreator 2021-07-13T14:09:22
#
#-------------------------------------------------

QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PowerChat_Client
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

CONFIG += c++11

SOURCES += \
        main.cpp \
        powerchatclient.cpp \
    logindialog.cpp \
    user.cpp \
    registerdialog.cpp \
    chatwidget.cpp \
    adddialog.cpp

HEADERS += \
        powerchatclient.h \
    logindialog.h \
    user.h \
    registerdialog.h \
    chatwidget.h \
    adddialog.h

FORMS += \
        powerchatclient.ui \
    logindialog.ui \
    registerdialog.ui \
    chatwidget.ui \
    adddialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc

DISTFILES += \
    PowerChat_Client.rc

OTHER_FILES+= \ PowerChat_Client.rc

RC_FILE = \
  PowerChat_Client.rc
