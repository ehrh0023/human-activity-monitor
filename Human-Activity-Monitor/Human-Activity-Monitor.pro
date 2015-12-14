#-------------------------------------------------
#
# Project created by QtCreator 2015-10-05T11:11:14
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

QMAKE_CXXFLAGS += /FS

TARGET = Humanactivity
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    OpenCVViewer.cpp \
    ../src/ColorProfiler.cpp \
    ../src/Filter.cpp \
    ../src/HandTracker.cpp \
    ../src/main.cpp \
    ../src/NaiveBayesClassifier.cpp \
    ../src/Region.cpp \
    ../src/RegionFinder.cpp \
    ../src/StatsGenerator.cpp \
    ../src/HandFinder.cpp \
    ../lib/qcustomplot/qcustomplot.cpp \
    StatsGraph.cpp \
    ../src/GaussianBayesClassifier.cpp

HEADERS  += mainwindow.h \
    OpenCVViewer.h \
    ../src/ColorProfiler.h \
    ../src/Filter.h \
    ../src/HandTracker.h \
    ../src/HSVRange.h \
    ../src/NaiveBayesClassifier.h \
    ../src/Region.h \
    ../src/RegionFinder.h \
    ../src/StatsGenerator.h \
    ../src/HandFinder.h \
    ../lib/qcustomplot/qcustomplot.h \
    StatsGraph.h \
    ../src/GaussianBayesClassifier.h \
    ../src/Stats.h

FORMS    += mainwindow.ui

win32-msvc2013:CONFIG(release, debug|release): LIBS += -L$$PWD/../lib/opencv/x86/vc12/lib/ -lopencv_videostab2410 \
                                                                                        -lopencv_ts2410 \
                                                                                        -lopencv_superres2410 \
                                                                                        -lopencv_stitching2410 \
                                                                                        -lopencv_contrib2410 \
                                                                                        -lopencv_nonfree2410 \
                                                                                        -lopencv_ocl2410 \
                                                                                        -lopencv_gpu2410 \
                                                                                        -lopencv_photo2410 \
                                                                                        -lopencv_objdetect2410 \
                                                                                        -lopencv_legacy2410 \
                                                                                        -lopencv_video2410 \
                                                                                        -lopencv_ml2410 \
                                                                                        -lopencv_calib3d2410 \
                                                                                        -lopencv_features2d2410 \
                                                                                        -lopencv_highgui2410 \
                                                                                        -lopencv_imgproc2410 \
                                                                                        -lopencv_flann2410 \
                                                                                        -lopencv_core2410


win32-msvc2013:CONFIG(debug, debug|release): LIBS += -L$$PWD/../lib/opencv/x86/vc12/lib/  -lopencv_videostab2410d \
                                                                                            -lopencv_ts2410d \
                                                                                            -lopencv_superres2410d \
                                                                                            -lopencv_stitching2410d \
                                                                                            -lopencv_contrib2410d \
                                                                                            -lopencv_nonfree2410d \
                                                                                            -lopencv_ocl2410d \
                                                                                            -lopencv_gpu2410d \
                                                                                            -lopencv_photo2410d \
                                                                                            -lopencv_objdetect2410d \
                                                                                            -lopencv_legacy2410d \
                                                                                            -lopencv_video2410d \
                                                                                            -lopencv_ml2410d \
                                                                                            -lopencv_calib3d2410d \
                                                                                            -lopencv_features2d2410d \
                                                                                            -lopencv_highgui2410d \
                                                                                            -lopencv_imgproc2410d \
                                                                                            -lopencv_flann2410d \
                                                                                            -lopencv_core2410d

win32-msvc2013: INCLUDEPATH += $$PWD/../lib/opencv/x86/vc12/lib/include
win32-msvc2013: DEPENDPATH += $$PWD/../lib/opencv/x86/vc12/lib/include


win32-msvc2015:CONFIG(release, debug|release): LIBS += -L$$PWD/../lib/opencv/x86/vc14/lib/ -lopencv_videostab2410 \
                                                                                        -lopencv_ts2410 \
                                                                                        -lopencv_superres2410 \
                                                                                        -lopencv_stitching2410 \
                                                                                        -lopencv_contrib2410 \
                                                                                        -lopencv_nonfree2410 \
                                                                                        -lopencv_ocl2410 \
                                                                                        -lopencv_gpu2410 \
                                                                                        -lopencv_photo2410 \
                                                                                        -lopencv_objdetect2410 \
                                                                                        -lopencv_legacy2410 \
                                                                                        -lopencv_video2410 \
                                                                                        -lopencv_ml2410 \
                                                                                        -lopencv_calib3d2410 \
                                                                                        -lopencv_features2d2410 \
                                                                                        -lopencv_highgui2410 \
                                                                                        -lopencv_imgproc2410 \
                                                                                        -lopencv_flann2410 \
                                                                                        -lopencv_core2410


else:win32-msvc2015:CONFIG(debug, debug|release): LIBS += -L$$PWD/../lib/opencv/x86/vc14/lib/  -lopencv_videostab2410d \
                                                                                            -lopencv_ts2410d \
                                                                                            -lopencv_superres2410d \
                                                                                            -lopencv_stitching2410d \
                                                                                            -lopencv_contrib2410d \
                                                                                            -lopencv_nonfree2410d \
                                                                                            -lopencv_ocl2410d \
                                                                                            -lopencv_gpu2410d \
                                                                                            -lopencv_photo2410d \
                                                                                            -lopencv_objdetect2410d \
                                                                                            -lopencv_legacy2410d \
                                                                                            -lopencv_video2410d \
                                                                                            -lopencv_ml2410d \
                                                                                            -lopencv_calib3d2410d \
                                                                                            -lopencv_features2d2410d \
                                                                                            -lopencv_highgui2410d \
                                                                                            -lopencv_imgproc2410d \
                                                                                            -lopencv_flann2410d \
                                                                                            -lopencv_core2410d

win32-msvc2015: INCLUDEPATH += $$PWD/../lib/opencv/x86/vc14/lib/include
win32-msvc2015: DEPENDPATH += $$PWD/../lib/opencv/x86/vc14/lib/include
