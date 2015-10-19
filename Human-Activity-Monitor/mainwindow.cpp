#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMainWindow>
#include "QtGui"
#include "QApplication"
#include "QHBoxLayout"
#include "QSlider"
#include "QSpinBox"
#include "QAbstractButton"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_textBrowser_destroyed()
{

}

void MainWindow::on_Velocity_actionTriggered(int action)
{

}

void MainWindow::on_Velocity_sliderMoved(int position)
{

   }

void MainWindow::on_pushButton_clicked()
{

}

void MainWindow::on_Frequency_actionTriggered(int action)
{

}
