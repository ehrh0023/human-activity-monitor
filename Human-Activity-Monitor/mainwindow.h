#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include "QtGui"
#include "QApplication"
#include "QHBoxLayout"
#include "QSlider"
#include "QSpinBox"
#include "QAbstractButton"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_textBrowser_destroyed();

    void on_pushButton_clicked();

    void on_actionOpen_File_triggered();

    void on_actionOpen_Webcam_triggered();

    void on_actionProcess_triggered();

    void on_actionSave_Stats_As_triggered();

    void on_actionSave_Video_As_triggered();

    void on_actionEnd_Processing_triggered();

private:
    Ui::MainWindow *ui;
    QTimer dataTimer;

    QString statsfile;
    QString vidfile;

};

#endif // MAINWINDOW_H
