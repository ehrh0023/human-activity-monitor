#include "OpenCVViewer.h"
#include <QtWidgets>
#include <opencv2/opencv.hpp>
#include <stdexcept>

OpenCVViewer::OpenCVViewer(QWidget *parent) :
    QWidget(parent),
    app(0) //"../assets/flap_blur.avi"
{
    timer = new QTimer(this);
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(display_scene()));
    timer->start(30);
}


void OpenCVViewer::display_scene()
{
    cv::Mat frame = app.update();
    showImage(frame);
}

bool OpenCVViewer::showImage( cv::Mat image )
{
    image.copyTo(original_img);

    cv::resize(original_img, original_img, cv::Size(size().width(), size().height()));

    if( original_img.channels() == 3)
        render_img = QImage((const unsigned char*)(original_img.data),
                              original_img.cols, original_img.rows,
                              original_img.step, QImage::Format_RGB888).rgbSwapped();
    else if( original_img.channels() == 1)
        render_img = QImage((const unsigned char*)(original_img.data),
                              original_img.cols, original_img.rows,
                              original_img.step, QImage::Format_Indexed8);
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

