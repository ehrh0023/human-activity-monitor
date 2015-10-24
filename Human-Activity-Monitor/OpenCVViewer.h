#pragma once

#include <QWidget>
#include <opencv2/opencv.hpp>

class OpenCVViewer : public QWidget
{
    Q_OBJECT
public:
    explicit OpenCVViewer(QWidget *parent = 0);

public slots:
    bool    showImage( cv::Mat image ); /// Used to set the image to be viewed

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
    QImage      render_img;           /// Qt image to be rendered
    cv::Mat     original_img;             /// original OpenCV image to be shown
};
