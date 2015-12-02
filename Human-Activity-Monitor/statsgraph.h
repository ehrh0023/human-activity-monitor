#pragma once

#include <QWidget>
#include <QTimer>
#include <opencv2/opencv.hpp>
#include "../lib/qcustomplot/qcustomplot.h"

class StatsGraph : public QCustomPlot
{
    Q_OBJECT
public:
    StatsGraph(QWidget *parent = 0);

    void realTimeData(double key, double value);

    void replace(double key, double value);
};
