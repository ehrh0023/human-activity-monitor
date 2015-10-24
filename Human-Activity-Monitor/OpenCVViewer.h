#pragma once

#include <QWidget>
#include <QTimer>
#include <opencv2/opencv.hpp>

class OpenCVViewer : public QWidget
{
    Q_OBJECT
public:
    OpenCVViewer(QWidget *parent = 0);

public slots:
    void display_scene();

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
    QImage      render_img;     /// Qt image to be rendered
    cv::Mat     original_img;   /// original OpenCV image to be shown
    bool    showImage( cv::Mat image ); /// Used to set the image to be viewed
    QTimer *timer;

    cv::VideoCapture cap;
};
