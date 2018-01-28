#-------------------------------------------------
#
# Project created by QtCreator 2018-01-28T22:12:19
#
#-------------------------------------------------

QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Carcassonne
TEMPLATE = app
CONFIG += c++11

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
    cardslot.cc \
    configuration.cc \
    gameui.cc \
    ignoreevents.cc \
    main.cc \
    card.cc \
    gameboard.cc \
    mygameengine.cc \
    playerdatabase.cc \
    pointcard.cc \
    statusscreen.cc

HEADERS += \
    cardslot.hh \
    configuration.hh \
    constants.hh \
    gameui.hh \
    IGameEngine.hh \
    IGameView.hh \
    ignoreevents.hh \
    card.hh \
    configuration.hh \
    constants.hh \
    IGameEngine.hh \
    IGameView.hh \
    gameboard.hh \
    gameconstants.hh \
    IGameEngine.hh \
    IGameView.hh \
    mygameengine.hh \
    playerdatabase.hh \
    pointcard.hh \
    statusscreen.hh

FORMS += \
    mainwindow.ui \
    config.ui \
    endscreen.ui \
    statusscreen.ui

RESOURCES += \
    peli.qrc \
    pelikortit.qrc
