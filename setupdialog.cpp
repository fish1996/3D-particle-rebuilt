﻿#include "setupdialog.h"
#include "addattrwindow.h"
#include "removeattrwindow.h"

#include <QLineEdit>
#include <QSpinBox>
#include <QPushButton>
#include <QCheckBox>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QComboBox>
#include <qDebug>
#include <QDataStream>
#include <QFile>
#include <QMessageBox>
#include <fstream>
#include <QUrl>
#include <QFileDialog>

bool setupdialog::isCreate = false;

void setupdialog::Add()
{
    addattrWindow = new addattrwindow();
    addattrWindow->show();
    connect(addattrWindow,SIGNAL(updateAttr(QString,premessage_t)),this,SLOT(updateAttr(QString,premessage_t)));
}

void setupdialog::closeEvent(QCloseEvent*)
{
    isCreate = false;
}

void setupdialog::Remove()
{

    removeattrWindow = new removeattrwindow(map,namelist);
    removeattrWindow->show();
    connect(removeattrWindow,SIGNAL(updateStyle()),this,SLOT(updateAttr()));
}

void setupdialog::updateAttr()
{
    disconnect(attrBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setValue(int)));
    attrBox->clear();
    connect(attrBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setValue(int)));
    for(int i=0;i<namelist->size();i++){
        attrBox->addItem((*namelist)[i],i);
    }
}

void setupdialog::updateAttr(QString str,premessage_t msg)
{
    namelist->push_back(str);
    (*map)[str] = msg;
    attrBox->addItem(str,namelist->size()-1);
}

setupdialog::setupdialog(int _sizex,int _sizey,QString p,QVector<QString>* n,map_t* m,double size,message_t* msg,QWidget* parent)
    :QWidget(parent)
{
    sizex = _sizex;
    sizey = _sizey;
    preattr = p;
    dpix = size;
    namelist = n;
    map = m;
    message = msg;

    isCreate = true;
    layout();
    setConnect();
}

void setupdialog::setConnect()
{
    connect(addAttrBtn,SIGNAL(clicked()),this,SLOT(Add()));
    connect(removeAttrBtn,SIGNAL(clicked()),this,SLOT(Remove()));
    connect(attrBox,SIGNAL(currentIndexChanged(int)),this,SLOT(setValue(int)));
    connect(chooseDir,SIGNAL(clicked()),this,SLOT(choose()));
}

void setupdialog::choose()
{
    QUrl url = QFileDialog::getExistingDirectoryUrl
            (this);
    QString path = url.toString().mid(8);
    if(path!="")fileDirText->setText(path);

}

setupdialog::~setupdialog()
{
    isCreate = false;
}

void setupdialog::layout()
{
    isChange = false;
   // setAttribute(Qt::WA_DeleteOnClose,true);
    line1 = new QLineEdit();
    line2 = new QLineEdit();
    line3 = new QLineEdit();
    fileDirText = new QLineEdit();
    fileDirText->setDisabled(true);
    intervalText = new QSpinBox();
    minRadiusText = new QSpinBox();
    maxRadiusText = new QSpinBox();
    scaleLengthText = new QSpinBox();
    placeXText = new QSpinBox();
    placeYText = new QSpinBox();

    attrBox = new QComboBox();
    for(int i=0;i<namelist->size();i++){
        attrBox->addItem((*namelist)[i],i);
    }

    chooseDir = new QPushButton(QStringLiteral("."));
    addAttrBtn = new QPushButton(QStringLiteral("+"));
    removeAttrBtn = new QPushButton(QStringLiteral("-"));
    okBtn = new QPushButton(QStringLiteral(" 确定 "));
    cancelBtn = new QPushButton(QStringLiteral(" 取消 "));

    detection = new QLabel(QStringLiteral("自适应阈值参数"));
    fileDir = new QLabel(QStringLiteral("用户文件夹"));
    interval = new QLabel(QStringLiteral("区间数"));
    minRadius = new QLabel(QStringLiteral("最小粒径"));
    maxRadius = new QLabel(QStringLiteral("最大粒径"));
    preAttr = new QLabel(QStringLiteral("预设参数"));
    scaleLength = new QLabel(QStringLiteral("比例尺长度"));
    place = new QLabel(QStringLiteral("比例尺位置"));
    place_x = new QLabel(QStringLiteral("x:"));
    place_y = new QLabel(QStringLiteral("y:"));
    um = new QLabel(QStringLiteral("um"));

    for(int i=0;i<HMAX;i++){
        hlayout[i] = new QHBoxLayout();
    }
    for(int i=0;i<VMAX;i++){
        vlayout[i] = new QVBoxLayout();
    }


    line1->setText(QString::number(message->detection1));
    line2->setText(QString::number(message->detection2));
    line3->setText(QString::number(message->detection3));

    maxRadiusText->setRange(1,100);
    scaleLengthText->setRange(10,2000);

    placeXText->setRange(1,sizex - 50);
    placeYText->setRange(1,sizey - 50);
    fileDirText->setText(message->path);
    placeXText->setValue(message->placex);
    placeYText->setValue(message->placey);
    scaleLengthText->setValue(message->size);
    minRadiusText->setValue(message->minRadius);
    maxRadiusText->setValue(message->maxRadius);
    intervalText->setValue(message->interval);
    attrBox->setCurrentText(preattr);

    hlayout[0]->addWidget(detection);
    hlayout[0]->addWidget(line1);
    hlayout[0]->addWidget(line2);
    hlayout[0]->addWidget(line3);

    hlayout[1]->addWidget(fileDir);
    hlayout[1]->addWidget(fileDirText);
    hlayout[1]->addWidget(chooseDir);

    hlayout[2]->addWidget(preAttr);
    hlayout[2]->addWidget(attrBox);
    hlayout[2]->addWidget(addAttrBtn);
    hlayout[2]->addWidget(removeAttrBtn);

    hlayout[3]->addWidget(minRadius);
    hlayout[3]->addWidget(minRadiusText);
    hlayout[3]->addWidget(maxRadius);
    hlayout[3]->addWidget(maxRadiusText);

    hlayout[4]->addWidget(interval);
    hlayout[4]->addWidget(intervalText);
    hlayout[4]->addWidget(scaleLength);
    hlayout[4]->addWidget(scaleLengthText);
    hlayout[4]->addWidget(um);

    hlayout[5]->addWidget(place);
    hlayout[5]->addWidget(place_x);
    hlayout[5]->addWidget(placeXText);
    hlayout[5]->addWidget(place_y);
    hlayout[5]->addWidget(placeYText);

    hlayout[6]->addStretch();
    hlayout[6]->addWidget(okBtn);
    hlayout[6]->addWidget(cancelBtn);

    vlayout[0]->addLayout(hlayout[0]);
    vlayout[0]->addLayout(hlayout[1]);
    vlayout[0]->addLayout(hlayout[2]);
    vlayout[0]->addLayout(hlayout[3]);
    vlayout[0]->addLayout(hlayout[4]);
    vlayout[0]->addLayout(hlayout[5]);
    vlayout[0]->addLayout(hlayout[6]);

    setWindowModality(Qt::ApplicationModal);
    //setWindowFlags(Qt::WindowStaysOnTopHint);
    setWindowTitle(QStringLiteral("设置"));

    setLayout(vlayout[0]);
}

void setupdialog::setValue(int i)
{
    isChange = true;
    QString name = (*namelist)[i];

    line1->setText((*map)[name].detection1);
    line2->setText((*map)[name].detection2);
    line3->setText((*map)[name].detection3);

    maxRadiusText->setRange(1,100);
    fileDirText->setText((*map)[name].path);

    minRadiusText->setValue((*map)[name].minRadius.toInt());
    maxRadiusText->setValue((*map)[name].maxRadius.toInt());
    intervalText->setValue((*map)[name].plotnum.toInt());

}
