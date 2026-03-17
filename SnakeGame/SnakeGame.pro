QT       += core gui widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    menuscreen.cpp \
    gamewidget.cpp \
    snake.cpp \
    food.cpp \
    highscore.cpp \
    highscoredialog.cpp

HEADERS += \
    mainwindow.h \
    menuscreen.h \
    gamewidget.h \
    snake.h \
    food.h \
    direction.h \
    gamemode.h \
    highscore.h \
    highscoredialog.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
