QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Clipboard/clipboardactionhandler.cpp \
    Clipboard/clipboardmanager.cpp \
    Clipboard/clipboardmonitor.cpp \
    Clipboard/myclipboardlistmodel.cpp \
    customtrayicon.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    Clipboard/clipboardactionhandler.h \
    Clipboard/clipboardmanager.h \
    Clipboard/clipboardmonitor.h \
    Clipboard/myclipboardinfo.h \
    Clipboard/myclipboardlistmodel.h \
    customtrayicon.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

LIBS +=  -luser32

INCLUDEPATH += Clipboard
