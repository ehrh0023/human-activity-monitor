#-------------------------------------------------
#
# Project created by QtCreator 2015-10-05T11:11:14
#
#-------------------------------------------------

QT       += core gui
QMAKE_CXXFLAGS += $$(CXXFLAGS)
QMAKE_CFLAGS += $$(CFLAGS)
QMAKE_LFLAGS += $$(LDFLAGS)

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Humanactivity
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    OpenCVViewer.cpp

HEADERS  += mainwindow.h \
    OpenCVViewer.h

FORMS    += mainwindow.ui

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../lib/opencv/build/mingw32/lib/ -lopencv_ts300
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../lib/opencv/build/mingw32/lib/ -lopencv_ts300d

INCLUDEPATH += $$PWD/../lib/opencv/build/include
DEPENDPATH += $$PWD/../lib/opencv/build/include
