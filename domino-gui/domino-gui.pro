#-------------------------------------------------
#
# Project created by QtCreator 2018-03-31T01:52:40
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = domino-gui
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


SOURCES += \
        main.cpp \
    game/board.cpp \
    game/piece.cpp \
    ui/game.cpp \
    ui/maindialog.cpp \
    ui/initialform.cpp \
    socket/client.cpp \
    socket/server.cpp \
    ui/servidorform.cpp \
    ui/clientform.cpp

HEADERS += \
    game/board.h \
    game/piece.h \
    ui/game.h \
    ui/maindialog.h \
    ui/initialform.h \
    socket/client.h \
    socket/server.h \
    socket/socket.h \
    ui/servidorform.h \
    ui/clientform.h \
    socket/settings.h

FORMS += \
    ui/game.ui \
    ui/maindialog.ui \
    ui/initialform.ui \
    ui/servidorform.ui \
    ui/clientform.ui

RESOURCES += \
    img.qrc
