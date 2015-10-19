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

    void on_Velocity_actionTriggered(int action);

    void on_pushButton_clicked();

    void on_Velocity_sliderMoved(int position);

    void on_Frequency_actionTriggered(int action);
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
