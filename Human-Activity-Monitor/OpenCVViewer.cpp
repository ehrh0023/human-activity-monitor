#include "OpenCVViewer.h"
#include <QtWidgets>
#include <opencv2/opencv.hpp>

OpenCVViewer::OpenCVViewer(QWidget *parent) :
    QWidget(parent)
{
}


bool OpenCVViewer::showImage( cv::Mat image )
{
    image.copyTo(original_img);

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

