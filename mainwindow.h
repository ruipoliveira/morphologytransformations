#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QLabel>
#include <QMessageBox>
#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "opencv/cv.h"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QString fileName;
    int typeBin;
    QKeyEvent *keyevent;

private:
    Ui::MainWindow *ui;
    void keyPressEvent(QKeyEvent * event);
   // bool killLoopFlag_;

private slots:
    void on_pushButton_clicked();
    void on_pushButton_3_clicked();
    int binarizationType();
    QImage IplImage2QImage(const IplImage *iplImage);
    // void killLoop() { killLoopFlag_ = true; }


};

#endif // MAINWINDOW_H
