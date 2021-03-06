﻿#include "setupwindow.h"
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QPainter>
#include <QDateTime>
#include <QFileDialog>
#include <qDebug>
#include <QComboBox>
#include <QMessageBox>

void imgAttr_t::setSize(int s)
{
    size = s;
}

void imgAttr_t::setWidth(int w)
{
    sizeW = w;
}

void imgAttr_t::setHeight(int h)
{
    sizeH = h;
}

void imgAttr_t::setCreateDate(QString d)
{
    createdate = d;
}

void imgAttr_t::setReviseDate(QString d)
{
    revisedate = d;
}

void imgAttr_t::setBackground(bool b)
{
    hasBackground = b;
}

int imgAttr_t::getSize()const
{
    return size;
}

int imgAttr_t::getWidth()const
{
    return sizeW;
}

int imgAttr_t::getHeight()const
{
    return sizeH;
}

bool imgAttr_t::isBackground()const
{
    return hasBackground;
}

QString imgAttr_t::getCreateDate()const
{
    return createdate;
}

QString imgAttr_t::getReviseDate()const
{
    return revisedate;
}

tabwindow::tabwindow(QTabWidget* parent):
    QTabWidget(parent)
{
    layout();
    setConnect();
}

void tabwindow::layout()
{

    status = "disconnected";
    imgAttr.setSize(0);
    imgAttr.setWidth(0);
    imgAttr.setHeight(0);
    imgAttr.setCreateDate("");
    imgAttr.setReviseDate("");
    imgAttr.setBackground(true);

    for(int i=0;i<HMAX;i++){
        hlayout[i] = new QHBoxLayout();
    }

    for(int i=0;i<VMAX;i++){
        vlayout[i] = new QVBoxLayout();
    }
    cameralayout();
    filelayout();

    addTab(cameraWindow,QStringLiteral("相机"));
    addTab(fileWindow,QStringLiteral("文件"));
}

void tabwindow::cameralayout()
{
    came = new camera();
    playBtn = new Button();
    multiPlayBtn = new Button();
    stopBtn = new Button();

    QIcon playico,multiplayico,stopico;
    playico.addFile(":/image/play.ico",QSize(32,32));
    multiplayico.addFile(":/image/mutiplay.ico",QSize(32,32));
    stopico.addFile(":/image/stop.ico",QSize(32,32));

    playBtn->SetButtonIcons(playico,playico,playico);
    multiPlayBtn->SetButtonIcons(multiplayico,multiplayico,multiplayico);
    stopBtn->SetButtonIcons(stopico,stopico,stopico);

    playBtn->setIconSize(QSize(32,32));
    playBtn->setFixedWidth(100);
    playBtn->setFlat(true);

    multiPlayBtn->setIconSize(QSize(32,32));
    multiPlayBtn->setFixedWidth(100);
    multiPlayBtn->setFlat(true);

    stopBtn->setIconSize(QSize(32,32));
    stopBtn->setFixedWidth(100);
    stopBtn->setFlat(true);

    prefix = new QLabel(QStringLiteral("后缀"));
    fps = new QLabel(QStringLiteral("帧率"));
    exposureTime = new QLabel(QStringLiteral("曝光时间"));
    chooseCamera = new QLabel(QStringLiteral("选择相机"));
    size = new QLabel(QStringLiteral("分辨率"));

    cameraGroupBox = new QGroupBox();
    prefixText = new QComboBox();
    chooseCameraText = new QComboBox();
    sizeText = new QComboBox();

    fpsBox = new QSpinBox();
    timeBox = new QSpinBox();

    cameraWindow = new QWidget();

    fpsBox->setRange(1,60);
    fpsBox->setValue(1);

    timeBox->setRange(0,30);
    timeBox->setValue(20);

    for(int i = 0;i < came->deviceNum;i++){
        chooseCameraText->addItem(QString::number(i+1));
    }

    sizeText->addItem(QStringLiteral("3262 x 2448"));
    sizeText->addItem(QStringLiteral("2592 x 1944"));
    sizeText->addItem(QStringLiteral("1600 x 1200"));
    sizeText->addItem(QStringLiteral("1280 x 960 "));
    sizeText->addItem(QStringLiteral("1024 x 768 "));
    sizeText->addItem(QStringLiteral("800  x 500 "));
    sizeText->addItem(QStringLiteral("640  x 480 "));

    prefixText->addItem(QStringLiteral("jpg"));
    prefixText->addItem(QStringLiteral("bmp"));
    prefixText->addItem(QStringLiteral("png"));

    hlayout[1]->addWidget(prefix);
    hlayout[1]->addWidget(prefixText);

    hlayout[2]->addWidget(fps);
    hlayout[2]->addWidget(fpsBox);

    hlayout[3]->addWidget(exposureTime);
    hlayout[3]->addWidget(timeBox);

    hlayout[4]->addWidget(chooseCamera);
    hlayout[4]->addWidget(chooseCameraText);

    hlayout[5]->addWidget(size);
    hlayout[5]->addWidget(sizeText);

    hlayout[6]->addStretch();
    hlayout[6]->addWidget(playBtn);
    hlayout[6]->addWidget(multiPlayBtn);
    hlayout[6]->addWidget(stopBtn);
    hlayout[6]->addStretch();

    vlayout[0]->addWidget(came);
    vlayout[0]->addLayout(hlayout[1]);
    vlayout[0]->addLayout(hlayout[2]);
    vlayout[0]->addLayout(hlayout[3]);
    vlayout[0]->addLayout(hlayout[4]);
    vlayout[0]->addLayout(hlayout[5]);
    vlayout[0]->addLayout(hlayout[6]);

    cameraGroupBox->setLayout(vlayout[0]);
    cameraGroupBox->setTitle(QStringLiteral("camera:") + status);
    vlayout[1]->addWidget(cameraGroupBox);
    cameraWindow->setLayout(vlayout[1]);

}

void tabwindow::updateTitle()
{
    cameraGroupBox->setTitle(QStringLiteral("camera:") + status);
}

void tabwindow::filelayout()
{
    filePicture = new QLabel();
    imgMessage = new QLabel();
    QPixmap img(path);
    img = img.scaled(QSize(width(),height()/4*3),Qt::KeepAspectRatio);
    filePicture->setPixmap(img);

    fileWindow = new QWidget();
    fileGroupBox = new QGroupBox();

    QString message = QStringLiteral("宽度:")
            + QString::number(imgAttr.getWidth())
            + QStringLiteral("\n高度:")
            + QString::number(imgAttr.getHeight())
            + QStringLiteral("\n大小:")
            + QString::number(1.0*imgAttr.getSize()/1024)
            + QStringLiteral("KB\n创建日期:")
            + imgAttr.getCreateDate()
            + QStringLiteral("\n修改日期")
            + imgAttr.getReviseDate();
    imgMessage->setText(message);

    vlayout[2]->addWidget(filePicture);
    vlayout[2]->addWidget(imgMessage);
    fileGroupBox->setLayout(vlayout[2]);
    fileGroupBox->setTitle(QStringLiteral("hologram"));
    vlayout[3]->addWidget(fileGroupBox);
    fileWindow->setLayout(vlayout[3]);
}

void tabwindow::setConnect()
{
    connect(fpsBox,SIGNAL(valueChanged(int)),came,SLOT(setFPS(int)));
    connect(sizeText,SIGNAL(currentIndexChanged(QString)),this,SLOT(setSize(QString)));
    connect(timeBox,SIGNAL(valueChanged(int)),came,SLOT(setTime(int)));
    connect(playBtn,SIGNAL(clicked()),came,SLOT(shot()));
    connect(multiPlayBtn,SIGNAL(clicked()),came,SLOT(multishot()));
    connect(stopBtn,SIGNAL(clicked()),came,SLOT(stopshot()));
    connect(prefixText,SIGNAL(currentIndexChanged(QString)),came,SLOT(updatePrefix(QString)));
    connect(chooseCameraText,SIGNAL(currentIndexChanged(int)),this,SLOT(updateIndex(int)));
}

void tabwindow::setSize(QString size)
{
    if(size.compare("3262 x 2448")==0){
        came->setSize(3262,2448);
    }
    else if(size.compare("2592 x 1944")==0){
        came->setSize(2592,1944);
    }
    else if(size.compare("1600 x 1200")==0){
        came->setSize(1600,1200);
    }
    else if(size.compare("1280 x 960 ")==0){
        came->setSize(1280,960);
    }
    else if(size.compare("800  x 600 ")==0){
        came->setSize(800,600);
    }
    else if(size.compare("1024 x 768 ")==0){
        came->setSize(1024,768);
    }
    else if(size.compare("640  x 480 ")==0){
        came->setSize(640,480);
    }
}

void tabwindow::updateIndex(int in)
{
    came->index = in;
    if(came->isOpen) {
        came->open();
    }
}

void tabwindow::setPath()
{
    QString filename = QFileDialog::getSaveFileName(this,
        tr("Save Image"),
        "",
        tr("*.bmp;; *.png;; *.jpg;; *.tif;; *.GIF")); //选择路径
    if(filename.isEmpty())
    {
        return;
    }
    else{
        pathText->setText(filename);
    }
}

setupwindow::setupwindow(QWidget* parent)
    :QWidget(parent)
{
    layout();
}

QString setupwindow::getPath()
{
    return Path;
}

void setupwindow::setPath(QString path)
{
    tabWindow->came->setPath(path);
}

void setupwindow::loadImg(QString path)
{
    Path = path;
    QPixmap img(path);
    QFileInfo info(path);
    QVector<QString>pathVector;
    for(int i = 0;i < info.path().size();i += 30) {
        pathVector.push_back(info.path().mid(i,30));
    }

    QString message = QStringLiteral("图片名：")
            + info.fileName()
            + QStringLiteral("\n路径:");
    for(int i = 0;i<pathVector.size();i++){
        message = message + pathVector[i] + "\n";
    }
    message += QStringLiteral("宽度:")
            + QString::number(img.width())
            + QStringLiteral("\n高度:")
            + QString::number(img.height())
            + QStringLiteral("\n大小:")
            + QString::number(1.0*info.size()/1024)
            + QStringLiteral("KB\n创建日期:")
            + info.lastModified().toString()
            + QStringLiteral("\n修改日期:")
            + info.created().toString();
    tabWindow->imgMessage->setText(message);
    tabWindow->setCurrentIndex(1);
    img = img.scaled(tabWindow->filePicture->size(),Qt::KeepAspectRatio);
    tabWindow->filePicture->setPixmap(img);
}


void setupwindow::layout()
{
    isOpen = false;
    isHide = false;
    SIZEW = 400*rebuilt::scale;
    SIZEH = 600*rebuilt::scale;
    setFixedWidth(350);

    for(int i=0;i<HMAX;i++){
        hlayout[i] = new QHBoxLayout();
    }

    for(int i=0;i<VMAX;i++){
        vlayout[i] = new QVBoxLayout();
    }


    glayout = new QGridLayout();
    glayout1 = new QGridLayout();
    tabWindow = new tabwindow();
    window = new QWidget();
    zmin = new QLabel(QStringLiteral("重建起点"));
    zmax = new QLabel(QStringLiteral("重建终点"));
    interval = new QLabel(QStringLiteral("平面间距"));
    lamda = new QLabel(QStringLiteral("重建波长"));
    dpix = new QLabel(QStringLiteral("像素尺寸"));
    cm[0] = new QLabel(QStringLiteral("cm"));
    cm[1] = new QLabel(QStringLiteral("cm"));
    mm = new QLabel(QStringLiteral("mm"));
    um = new QLabel(QStringLiteral("um"));
    nm = new QLabel(QStringLiteral("nm"));

    zminText = new QLineEdit();
    zmaxText = new QLineEdit();
    intervalText = new QLineEdit();
    lamdaText = new QLineEdit();
    MagText = new QLineEdit();
    dpixText = new QLineEdit();

    oneKeyBtn = new QPushButton(QStringLiteral("智能测量"));
    runBtn = new QPushButton(QStringLiteral("全息重建"));
    detectionBtn = new QPushButton(QStringLiteral("颗粒识别"));
    locationBtn = new QPushButton(QStringLiteral("颗粒定位"));
    figplotBtn = new QPushButton(QStringLiteral("景深扩展"));


    glayout->addWidget(zmin,1,1);
    glayout->addWidget(zminText,1,2);
    glayout->addWidget(cm[0],1,3);
    glayout->addWidget(lamda,1,4);
    glayout->addWidget(lamdaText,1,5);
    glayout->addWidget(nm,1,6);
    glayout->addWidget(interval,2,1);
    glayout->addWidget(intervalText,2,2);
    glayout->addWidget(mm,2,3);
    glayout->addWidget(dpix,2,4);
    glayout->addWidget(dpixText,2,5);
    glayout->addWidget(um,2,6);
    glayout->addWidget(zmax,3,1);
    glayout->addWidget(zmaxText,3,2);
    glayout->addWidget(cm[1],3,3);

    glayout->setMargin(10);

    glayout1->setMargin(10);
    glayout1->addWidget(oneKeyBtn,1,1,2,2);
    oneKeyBtn->setFixedSize(120,60);
    glayout1->addWidget(runBtn,1,3,1,1);
    runBtn->setFixedWidth(80);
    glayout1->addWidget(figplotBtn,1,4,1,1);
    figplotBtn->setFixedWidth(80);
    glayout1->addWidget(detectionBtn,2,3,1,1);
    figplotBtn->setFixedWidth(80);
    glayout1->addWidget(locationBtn,2,4,1,1);
    figplotBtn->setFixedWidth(80);

    vlayout[1]->addLayout(glayout);
    vlayout[1]->addLayout(glayout1);
    vlayout[1]->setMargin(0);
    window->setLayout(vlayout[1]);

    QWidget* title;
    dock = new QDockWidget(QStringLiteral("参数"),this);
    dock->setFeatures(QDockWidget::DockWidgetFloatable);
    if(title = dock->titleBarWidget()){
        title->setStyleSheet("text-align: left;"
                                           "background: rgb(255,0,0);"
                                           "padding-left:5px;"
                                           );
    }
    dock->setWidget(window);

    vlayout[0]->addWidget(tabWindow);
    vlayout[0]->addWidget(dock);
    vlayout[0]->setMargin(0);
    vlayout[0]->setSpacing(0);
    vlayout[0]->setStretch(0,80);
    vlayout[0]->setStretch(1,20);


    intervalText->setText("1");


    connect(tabWindow,SIGNAL(currentChanged(int)),
            this,SLOT(indexChanged(int)));
    setLayout(vlayout[0]);
}

void setupwindow::paintEvent(QPaintEvent*)
{
    QPainter paint(this);
    paint.setPen(QColor(255,255,255));
    paint.setBrush(QColor(255,255,255));
    paint.drawRect(rect());
}

void setupwindow::indexChanged(int index)
{
    if(index == 1) {
        isHide = true;
    }
    else if(index == 0) {
        isHide = false;
    }
    if(isHide && isOpen) {
        tabWindow->came->close();
    }
    if(!isHide && isOpen) {
        tabWindow->came->open();
    }
}


void setupwindow::play()
{
    if(tabWindow->came->deviceNum == 0){
        return;
    }
    if(!isOpen) {
        isOpen = true;
        tabWindow->status = "connected";
        if(tabWindow->came->open()){
            tabWindow->updateTitle();
        }
    }
    else {
        isOpen = false;
        tabWindow->status = "disconnected";
        tabWindow->came->close();
        tabWindow->updateTitle();
    }
}

void setupwindow::findCamera()
{
    tabWindow->came->updateDeviceNum();
    QMessageBox::about(0,"Message",QStringLiteral("当前检测到的相机个数为：")+
                       QString::number(tabWindow->came->deviceNum));
}

void setupwindow::closeCamera(){
    if(tabWindow->came->deviceNum == 0)return;
    if(isOpen) {
        tabWindow->came->close();
    }
}
