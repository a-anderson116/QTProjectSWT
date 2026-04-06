QT       += core gui widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

TARGET   = SpaceInvaders
TEMPLATE = app

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    menuscreen.cpp \
    gamewidget.cpp \
    player.cpp \
    invader.cpp \
    bullet.cpp \
    bunker.cpp \
    mysteryship.cpp \
    highscore.cpp \
    highscoredialog.cpp

HEADERS += \
    mainwindow.h \
    menuscreen.h \
    gamewidget.h \
    player.h \
    invader.h \
    bullet.h \
    bunker.h \
    mysteryship.h \
    highscore.h \
    highscoredialog.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
