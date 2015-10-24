#-------------------------------------------------
#
# Project created by QtCreator 2015-10-05T11:11:14
#
#-------------------------------------------------

QT       += core gui


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Humanactivity
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    OpenCVViewer.cpp

HEADERS  += mainwindow.h \
    OpenCVViewer.h

FORMS    += mainwindow.ui

win32: LIBS += -L$$PWD/../lib/opencv/build/mingw32/bin/ -lopencv_calib3d300 \
                                                        -lopencv_core300 \
                                                        -lopencv_features2d300 \
                                                        -lopencv_flann300 \
                                                        -lopencv_highgui300 \
                                                        -lopencv_imgcodecs300 \
                                                        -lopencv_imgproc300 \
                                                        -lopencv_ml300 \
                                                        -lopencv_objdetect300 \
                                                        -lopencv_photo300 \
                                                        -lopencv_shape300 \
                                                        -lopencv_stitching300 \
                                                        -lopencv_superres300 \
                                                        -lopencv_video300 \
                                                        -lopencv_videoio300 \
                                                        -lopencv_videostab300 \
                                                        -lopencv_ffmpeg300
INCLUDEPATH += $$PWD/../lib/opencv/build/include
DEPENDPATH += $$PWD/../lib/opencv/build/include
