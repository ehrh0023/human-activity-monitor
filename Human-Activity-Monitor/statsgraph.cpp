#include "StatsGraph.h"

StatsGraph::StatsGraph(QWidget *parent):
    QCustomPlot(parent)
{
    addGraph(); // blue line
    graph(0)->setPen(QPen(Qt::blue));
    graph(0)->setBrush(QBrush(QColor(240, 255, 200)));
    graph(0)->setAntialiasedFill(false);

    xAxis->setAutoTickStep(false);
    xAxis->setTickStep(2);
    axisRect()->setupFullAxesBox();

    xAxis->setRange(10, 10, Qt::AlignRight);

    // make left and bottom axes transfer their ranges to right and top axes:
    connect(xAxis, SIGNAL(rangeChanged(QCPRange)), xAxis2, SLOT(setRange(QCPRange)));
    connect(yAxis, SIGNAL(rangeChanged(QCPRange)), yAxis2, SLOT(setRange(QCPRange)));
 }

void StatsGraph::realTimeData(double key, double value)
{
    // add data to lines:
    graph(0)->addData(key, value);

    // remove data of lines that's outside visible range:
    graph(0)->removeDataBefore(key-10);

    // rescale value (vertical) axis to fit the current data:
    graph(0)->rescaleValueAxis();

    // make key axis range scroll with the data (at a constant range size of 10):
    xAxis->setRange(key-1, 10, Qt::AlignRight);
    replot();
}

void StatsGraph::replace(double key, double value)
{
    // remove dupe data:
    graph(0)->removeData(key);

    // add data to lines:
    graph(0)->addData(key, value);

    // rescale value (vertical) axis to fit the current data:
    graph(0)->rescaleValueAxis();
}
