﻿#ifndef SHOWWINDOW_H
#define SHOWWINDOW_H

#include "basewindow.h"
#include "scale.h"
#include <QWidget>
#include <QTabWidget>
#include <QLabel>

#include "openglwindow.h"
#include "attr.h"
class plotwindow;
class QPaintEvent;
class tablewindow;
class QVBoxLayout;
class QHBoxLayout;
class QComboBox;
class QAction;
class QMenu;

class picture : public QLabel
{
    Q_OBJECT
protected:
    void closeEvent(QCloseEvent* event);
};


class showwindow : public basewindow
{
    Q_OBJECT
private:

    int* SIZEW;
    int* SIZEH;

    float x;

    void layout();
    bool drawWhite;

    picture* pic;
public:
    int original_w;
    int original_h;
    bool ishold;
    float pixelSize;
    bool isCalculate;
    bool* isInverse;
    bool isDraw;
    float beginx,beginy;
    float endx,endy;
    float getRatio();
    QPixmap pix;
    QWidget* parent;
    float ratio;
    int width;
    int height;
    int w,h;
    QString path;
    void reloadImg();
    QPoint point;
    bool isDrawWhite();
    void isDrawWhite(bool);

    void setPixelSize(float s);
    void reset();
    void setI(bool*);

    static bool isShow;
    showwindow(basewindow* parent,int* SIZEW,int* SIZEH);
    void loadImg(QString p);
    void zoomIn();
    void zoomOut();
    void original();
    void fullScreen();
protected:
    void paintEvent(QPaintEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mousePressEvent(QMouseEvent *event);
signals:
    void send(float);
};

class framewindow : public basewindow
{
    Q_OBJECT
private:
    int* SIZEW;
    int* SIZEH;
    QMenu* menu;
    QAction* saveAction;
    scale* sca;
    QString path;

    bool* isInverse;
    int imgIndex;
    int total;
    int state;
    int index;
public:
   // void setMsg(message_t& msg);
    int sizeX();
    int sizeY();
    void setPlace(int,int);
    void setState(int);
    bool isDrawWhite();
    void isDrawWhite(bool);
    bool isShowWhite();
    void isShowWhite(bool);
    QComboBox* getBox();
    void setFigIndex(int);
    void setDetectIndex(int);
    showwindow* showWindow;
    QComboBox* attrBox;
    void addItem();
    void setIndex(int);
    void setCalculate(bool);
    bool getCalculate();
    void reset();
    scale* getSca();
    void zoomIn();
    void zoomOut();
    void original();
    void fullScreen();
    void addItem(int);
    showwindow* getshowwindow();
    void setNum(int);
    void setTotalNum(int);
    void setPath(QString p);
    QString getPath();
    void reloadImg();
    void setPixelSize(double p);
    void setI(bool*);
    void setScale(int*);
    framewindow(basewindow* parent = 0);
protected:
    void wheelEvent(QWheelEvent *event);
    void paintEvent(QPaintEvent* event);
    void contextMenuEvent(QContextMenuEvent* event);
private slots:
    void save();

signals:
    void send(float);
};

class showtabwindow : public QTabWidget
{
    Q_OBJECT
private:
    void layout();
    int num;
    int count;
    bool isCalculate;
    int Index;
    QString prefix;

public:
  //  void setMsg(message_t& msg);
    void setPlace(int x,int y);
    void setPrefix(QString);
    int sizeX();
    int sizeY();
    bool isOneKey;
    plotwindow* countwindow;
    framewindow* frameWindow;
    framewindow* figWindow;
    framewindow* detectWindow;
    OpenGLView* openglWindow;
    showtabwindow(QTabWidget* parent = 0);
    void addItem(int);
    void moveLeft();
    void moveRight();
    void moveFront();
    void moveBack();
    int getCurX();
    int getCurY();
    bool isDrawWhite();
    bool isShowWhite();
    void zoomIn();
    void zoomOut();
    void reset();
    void save(int,QString,QString,bool);
    void original();
    void isShowWhite(bool);
    void isDrawWhite(bool);
    void setPath(QString);
    void fullScreen();
    void setTotalNum(int);
    void reloadImg();
    void setI(bool*);
    void setInverse(bool);
    void setScale(int*);
    void setCalculate(bool);
    bool getCalculate();
    void setPlotNum(int n);
    void setWavelength(double w);
    void setPixelSize(double p);
    void paintEvent(QPaintEvent *);
    void setRadius(double* min,double* max);
    QString getPath();

public slots:
    scale* getSca();
    void initDetect();
    void initPlot();
    void initLocate(position** pos,position** ipos,int num,int size,
                                   double**diameterfre,double** idiameterfre,
                                   double*diametermin,double*idiametermin,
                                   double*diametermax,double*idiametermax,
                                   int* pointnum,int * ipointnum,bool maxflag,bool imaxflag,double*meandiameter,double*imeandiameter);
    void indexChanged(int);
    void setIndex(int);
    void initIndex(int);
    void setNum(int);
};

#endif // SHOWWINDOW_H
