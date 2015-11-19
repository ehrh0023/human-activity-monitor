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

    void on_customPlot_destroyed();

private:
    Ui::MainWindow *ui;
    QTimer dataTimer;
};

#endif // MAINWINDOW_H
