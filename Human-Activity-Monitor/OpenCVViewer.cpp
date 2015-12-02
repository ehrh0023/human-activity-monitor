#include "OpenCVViewer.h"
#include <QtWidgets>
#include <opencv2/opencv.hpp>
#include <stdexcept>

OpenCVViewer::OpenCVViewer(QWidget *parent) :
    QWidget(parent),
    shouldProcess(false),
    sampidx(0),
    app(0) //"../assets/flap_blur.avi"
{
    timer = new QTimer(this);
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(display_scene()));
    timer->start(30);
}


void OpenCVViewer::display_scene()
{
    cv::Mat frame = app.next_frame();

    if(frame.empty())
    {
        app.restart_video();
        frame = app.next_frame();
    }

    if(!frame.empty())
    {
        if(shouldProcess)
        {
            samples[sampidx] = app.process(frame);

            // calculate two new data points:
            double key = samples[sampidx].time;
            double freq = samples[sampidx].frequency;
            double vel = samples[sampidx].velocity;

            if(freq != 0)
            {
                int frame_count = 1/(0.0333333 * freq);
                for(int i = frame_count; i > 1; i--)
                {
                    int j = (sampidx - i) % 30;

                    statsVel->replace(samples[j].time,vel);
                    statsFreq->replace(samples[j].time,freq);
                }
            }

            statsVel->realTimeData(key,vel);
            statsFreq->realTimeData(key,freq);

            sampidx = ++sampidx % 30;
        }
        if(app.is_cam() && writer.isOpened())
        {
            writer.write(frame);
        }
        showImage(frame);
    }
}

bool OpenCVViewer::showImage( cv::Mat image )
{
    if(image.empty())
        return false;

    float width  = image.cols;
    float height = image.rows;
    float percentWidth  = width / size().width();
    float percentHeight = height / size().height();

    if(percentWidth > percentHeight)
    {
        width  /= percentWidth;
        height /= percentWidth;
        cv::resize(image, image, cv::Size(width, height));
    }
    else
    {
        width  /= percentHeight;
        height /= percentHeight;
        cv::resize(image, image, cv::Size(width, height));
    }

    int startX = (size().width() - width) / 2;
    int startY = (size().height() - height) / 2;

    cv::Mat display( cv::Size(size().width(), size().height()), image.type(), cv::Scalar(180,180,180));
    image.copyTo(display(cv::Rect(startX, startY, width, height)));//.colRange(startX, width-1));

    if( display.channels() == 3)
        render_img = QImage((const unsigned char*)(display.data),
                              display.cols, display.rows,
                              display.step, QImage::Format_RGB888).rgbSwapped();
    else if( display.channels() == 1)
        render_img = QImage((const unsigned char*)(display.data),
                              display.cols, display.rows,
                              display.step, QImage::Format_Indexed8);
    else
        return false;

    repaint();

    return true;
}

void OpenCVViewer::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawImage(0, 0, render_img);
}

HandTracker& OpenCVViewer::getHandTracker()
{
    return app;
}


void OpenCVViewer::setProcessing(bool on)
{
    shouldProcess = on;
}

void OpenCVViewer::setVideoSave(std::string filename)
{
    writer.open(filename, CV_FOURCC('M','P','E','G'), 30, app.capture_size(), true);
}

void OpenCVViewer::releaseSaveVideo()
{
    writer.release();
}
