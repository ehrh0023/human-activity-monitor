#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "../lib/qcustomplot/qcustomplot.h"
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->opencv_display->statsVel = ui->customPlot_vel;
    ui->opencv_display->statsFreq = ui->customPlot_freq;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_textBrowser_destroyed()
{

}

void MainWindow::on_pushButton_clicked()
{
    QString filename=QFileDialog::getOpenFileName(
            this,
            tr("Open File"),
            "C://",
            "All Files (*.*);;Text File (*.txt);;Music File(*.mp3)");

    QMessageBox::information(this,tr("File Name"),filename);
}

void MainWindow::on_actionOpen_File_triggered()
{
    QString filename=QFileDialog::getOpenFileName(
            this,
            tr("Open File"),
            "C://",
            "All Files (*.*);;AVI File (*.avi);;MP4 File(*.mp4)");

    ui->opencv_display->getHandTracker().switch_source(filename.toUtf8().constData());
}

void MainWindow::on_actionOpen_Webcam_triggered()
{
    ui->opencv_display->getHandTracker().switch_source(0);

}

void MainWindow::on_actionProcess_triggered()
{
    if(statsfile.isEmpty())
    {
        statsfile=QFileDialog::getSaveFileName(
                this,
                tr("Save Stats"),
                "C://",
                "CSV File (*.csv)");
    }
    if(vidfile.isEmpty())
    {
        vidfile=QFileDialog::getSaveFileName(
                this,
                tr("Save Video"),
                "C://",
                "AVI File (*.avi)");
    }

}

void MainWindow::on_actionSave_Stats_As_triggered()
{
    statsfile=QFileDialog::getSaveFileName(
            this,
            tr("Save Stats"),
            "C://",
            "CSV File (*.csv)");

}

void MainWindow::on_actionSave_Video_As_triggered()
{
    vidfile=QFileDialog::getSaveFileName(
            this,
            tr("Save Video"),
            "C://",
            "AVI File (*.avi)");

}
