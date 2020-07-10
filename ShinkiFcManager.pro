QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++14

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += \
    ../HyoLibrary/mingw73_32/include \

SOURCES += \
    BoardItemDelegate.cpp \
    BoardModel.cpp \
    Match.cpp \
    MatchDetailDialog.cpp \
    Player.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    BoardItemDelegate.h \
    BoardModel.h \
    Match.h \
    MatchDetailDialog.h \
    Player.h \
    mainwindow.h

FORMS += \
    MatchDetailDialog.ui \
    mainwindow.ui

TRANSLATIONS += \
    ShinkiFcManager_ko.ts \
    ShinkiFcManager_en.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../HyoLibrary/mingw73_32/lib/ -lxlnt
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../HyoLibrary/mingw73_32/lib/ -lxlntd

win32:CONFIG(release, debug|release): DESTDIR = $$PWD/../SHINKI-FC-Manager_release
else:win32:CONFIG(debug, debug|release): DESTDIR = $$PWD/../SHINKI-FC-Manager_debug
