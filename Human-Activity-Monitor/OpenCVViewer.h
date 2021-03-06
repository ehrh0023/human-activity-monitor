#pragma once

#include <QWidget>
#include <QTimer>
#include <opencv2/opencv.hpp>
#include "../src/HandTracker.h"
#include "StatsGraph.h"

class OpenCVViewer : public QWidget
{
    Q_OBJECT
public:
    OpenCVViewer(QWidget *parent = 0);

    HandTracker& getHandTracker();

    StatsGraph* statsVel;
    StatsGraph* statsFreq;

    void setProcessing(bool on);

    void setVideoSave(std::string filename);

    void releaseSaveVideo();

public slots:
    void display_scene();

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
    QImage      render_img;     /// Qt image to be rendered
    bool    showImage( cv::Mat image ); /// Used to set the image to be viewed
    QTimer *timer;

    HandTracker app;
    bool shouldProcess;

    MovementSample samples[30];
    int sampidx;


    cv::VideoWriter writer;
};
