﻿
#ifndef CAMERA_H
#define CAMERA_H

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/contrib/contrib.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <QImage>
#include <QWidget>
#include <QThread>
#include <string>
#include <vector>
class QLabel;
class QHBoxLayout;
using namespace cv;
using namespace std;


class camera : public QWidget
{
    Q_OBJECT
private:
    bool flag;
    Mat image;
    int count;
    int count1;
    int cur;
    int total;
    QTimer *timer;
    QHBoxLayout* hlayout;
    VideoCapture capture;
    QPixmap* pixmap;
    QString path;
    QString prefix;
    double fps;
    double time;
    bool multiflag;

    void setImage(QImage img);
    void saveImage();
protected:
    void paintEvent(QPaintEvent *event);
public:
    int index;
    bool isOpen;
    int deviceNum;
    void updateDeviceNum();
    QImage cvMat2QImage(const cv::Mat& mat);
    void setPath(QString p);
    camera(QWidget* parent = 0);
    bool open();
    void close();

private slots:
    void changeState();
public slots:
    void shot();
    void setFPS(int num);
    void setTime(int num);
    void setSize(int w,int h);
    void updatePrefix(QString);
    void multishot();
    void stopshot();
signals:
    void changePath(QString p);
};

#endif // CAMERA_H


