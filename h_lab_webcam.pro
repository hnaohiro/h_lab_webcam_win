#-------------------------------------------------
#
# Project created by QtCreator 2013-04-21T15:24:17
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = h_lab_webcam
TEMPLATE = app

CONFIG += static

RC_FILE = h_lab_webcam.rc

SOURCES += main.cpp\
        mainwindow.cpp \
    QWsSocket.cpp \
    QWsServer.cpp \
    dialogconfig.cpp

HEADERS  += mainwindow.h \
    QWsSocket.h \
    QWsServer.h \
    dialogconfig.h

FORMS    += mainwindow.ui \
    dialogconfig.ui

RESOURCES += \
    resources.qrc

OTHER_FILES += \
    h_lab_webcam.rc
