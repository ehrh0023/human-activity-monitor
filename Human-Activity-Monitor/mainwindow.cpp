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

    ui->actionEnd_Processing->setEnabled(false);
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
    ui->actionSave_Video_As->setEnabled(false);
}

void MainWindow::on_actionOpen_Webcam_triggered()
{
    ui->opencv_display->getHandTracker().switch_source(0);
    ui->actionSave_Video_As->setEnabled(true);
}

void MainWindow::on_actionProcess_triggered()
{
    if(statsfile.isEmpty())
    {
        on_actionSave_Stats_As_triggered();
    }

    if(vidfile.isEmpty())
    {
        on_actionSave_Video_As_triggered();
    }

    ui->opencv_display->setProcessing(true);
    ui->opencv_display->getHandTracker().restart_video();

    ui->actionProcess->setEnabled(false);
    ui->actionSave_Stats_As->setEnabled(false);
    ui->actionSave_Video_As->setEnabled(false);
    ui->actionEnd_Processing->setEnabled(true);

     ui->actionOpen_File->setEnabled(false);
     ui->actionOpen_Webcam->setEnabled(false);
}

void MainWindow::on_actionSave_Stats_As_triggered()
{
    statsfile=QFileDialog::getSaveFileName(
            this,
            tr("Save Stats"),
            "C://",
            "CSV File (*.csv)");

    ui->opencv_display->getHandTracker().set_stats_file(statsfile.toUtf8().constData());
}

void MainWindow::on_actionSave_Video_As_triggered()
{
    HandTracker app = ui->opencv_display->getHandTracker();
    if(app.is_cam())
    {
        vidfile=QFileDialog::getSaveFileName(
                this,
                tr("Save Video"),
                "C://",
                "MPEG File (*.mpg)");

        ui->opencv_display->setVideoSave(vidfile.toUtf8().constData());
    }
}

void MainWindow::on_actionEnd_Processing_triggered()
{
    ui->opencv_display->setProcessing(false);

    ui->actionProcess->setEnabled(true);
    ui->actionSave_Stats_As->setEnabled(true);
    ui->actionEnd_Processing->setEnabled(false);

    HandTracker app = ui->opencv_display->getHandTracker();
    if(app.is_cam())
    {
        ui->opencv_display->releaseSaveVideo();
        ui->actionSave_Video_As->setEnabled(true);
    }
    statsfile = "";
    vidfile = "";

    ui->actionOpen_File->setEnabled(true);
    ui->actionOpen_Webcam->setEnabled(true);
}
