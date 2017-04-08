﻿#include "plotwindow.h"
#include <QPainter>
#include <QMouseEvent>
#include <qDebug>
#include <QContextMenuEvent>
#include <QMenu>
#include <QFileDialog>
#include <QTextEdit>

plotwindow::plotwindow(basewindow* parent)
    :basewindow(parent)
{
    SIZEW = rect().width();
    SIZEH = rect().height();
    XMIN = 50;
    XMAX = SIZEW - 50;
    YMIN = 30;
    YMAX = SIZEH - 30;
    isDraw = false;
    index = 0;

    isFirstDraw = true;
    currentRect = -1;
    layout();
    rectVector = new Rect[10];
    x = 0;
    num = 10;
    for(int i=0;i<x;i++){

        Rect r(XMIN + i*(XMAX-XMIN)/x,YMAX-(YMAX-YMIN)*diameterfre[index][i],(XMAX-XMIN)/x - 1,(YMAX-YMIN)*diameterfre[index][i]);
        rectVector[i] = r;
    }
}

void plotwindow::setPlotNum(int n)
{
    x = n;
}

void plotwindow::layout()
{
    isInverse = false;
    menu = new QMenu();
  //  pixmap = new QPixmap();
    saveAsImageAction = new QAction(this);
    saveAsExcelAction = new QAction(this);
    QColor color1(0,255,255,230);
    QColor color2(64,224,208,230);
    QColor color3(72,209,204,230);
    QColor color4(0,206,209,230);
    QColor color5(175,238,238,230);
    QColor color6(176,224,230,230);
    QColor color7(173,224,230,230);
    colorVector.push_back(color1);
    colorVector.push_back(color2);
    colorVector.push_back(color3);
    colorVector.push_back(color4);
    colorVector.push_back(color5);
    colorVector.push_back(color6);
    colorVector.push_back(color7);

    saveAsImageAction->setText(QStringLiteral("导出数据图"));
    saveAsExcelAction->setText(QStringLiteral("导出数据表"));

    connect(saveAsImageAction,SIGNAL(triggered()),this,SLOT(saveAsImage()));
    connect(saveAsExcelAction,SIGNAL(triggered()),this,SLOT(saveAsExcel()));

}

void plotwindow::loadTable(int r, int c)
{
    row = r;
    col = c;
    table = new QString*[row];
    for(int i = 0;i < col;i++) {
        table[i] = new QString[col];
    }
}

void plotwindow::saveAsExcel()
{
    //构建html文档内容
    QString filepath = QFileDialog::getSaveFileName(this, tr("Save as..."),
            QString(), tr("EXCEL files (*.xls *.xlsx);;HTML-Files (*.htm *.html);;"));
    if(filepath=="")return;
    saveExcel(filepath,index);
}

bool plotwindow::Rect::isOn(int x,int y)
{
    return x>=beginx && x<=beginx + sizex
            && y>=beginy && y<=beginy + sizey;
}

void plotwindow::setIndex(int i)
{
    if(i < 0 || i > imgnum) {
        isDraw = false;
        update();
        return;
    }
    isDraw = true;
    index = i;
    update();
}

plotwindow::Rect::Rect(int bx,int by,int sx,int sy):
    beginx(bx),beginy(by),sizex(sx),sizey(sy)
{

}

void plotwindow::setAttr(double** _diameterfre,double** _idiameterfre,
                         double* _diametermin,double* _idiametermin,
                         double* _diametermax,double* _idiametermax,
                         int n,position** p,position** ip,int* num,int* inum)
{

    pos = p;
    pointnum = num;
    imgnum = n;
    diameterfre =_diameterfre;
    diametermin = _diametermin;
    diametermax = _diametermax;
    ipos = ip;
    ipointnum = inum;
    idiameterfre =_idiameterfre;
    idiametermin = _idiametermin;
    idiametermax = _idiametermax;
    update();
}

plotwindow::Rect::Rect() { }

void plotwindow::mousePressEvent(QMouseEvent *event)
{
    int _x = event->pos().x();
    int _y = event->pos().y();
    for(int i=0;i<x;i++){
      //  qDebug()<<"i="<<i;
        if(rectVector[i].isOn(_x,_y)){
        //    qDebug()<<"isOn"<<i;
            if(currentRect!=i){
                currentRect = i;
                update();

            }
            return;
        }
    }
    currentRect = -1;
    update();
}

void plotwindow::contextMenuEvent(QContextMenuEvent* event)
{
    menu->clear();
    menu->addAction(saveAsImageAction);
    menu->addAction(saveAsExcelAction);

    menu->exec(QCursor::pos());
    event->accept();
}

void plotwindow::setInverse(bool is)
{
    isInverse = is;
    update();
}

void plotwindow::paintEvent(QPaintEvent* e)
{
    if(!isDraw)return;

    SIZEW = rect().width();
    SIZEH = rect().height();
    XMIN = 50;
    XMAX = SIZEW - 50;
    YMIN = 30;
    YMAX = SIZEH - 30;

    if(!isInverse) {
        qDebug()<<"noinverse";
        for(int i=0;i<x;i++){
            Rect r(XMIN + i*(XMAX-XMIN)/x,YMAX-(YMAX-YMIN)*diameterfre[index][i],(XMAX-XMIN)/x - 1,(YMAX-YMIN)*diameterfre[index][i]);
            rectVector[i] = r;
        }
        float step = 0.1f;
        float length;
        if(num!=0)length = (YMAX - YMIN)/num;
        else length = 0;
        QPainter paint(this);
        paint.setPen(Qt::NoPen);
        paint.setBrush(QColor(255,255,255));
        paint.drawRect(0,0,SIZEW,SIZEH);
        paint.setPen(QColor(0,0,0));
        paint.drawText(XMAX/2,24,QStringLiteral("粒径分布直方图"));
        paint.drawText(XMAX/2,YMAX + 20,QStringLiteral("粒径区间/um"));
        paint.drawText(40,YMAX + 20,QStringLiteral("频率"));
        paint.drawLine(XMIN,YMIN,XMIN,YMAX);
        paint.drawLine(XMIN,YMAX,XMAX,YMAX);
        paint.drawText(20,YMAX + 20,QString::number(0));
        for(int i = 1;i<=num;i++){
            paint.drawText(20,YMAX-i*length,QString::number(step*(i)));
        }
        double s = (diametermax[index] - diametermin[index])/(x);
        for(int i=0;i<=x;i++){

            paint.drawText(XMIN + i*(XMAX-XMIN)/x,YMAX+10,QString::number((int)(diametermin[index]+i*s)));
        }

        paint.setPen(QColor(100,100,100));
        for(int i = 1;i<=num;i++){
            paint.drawLine(XMIN,YMAX-i*length,XMAX,YMAX-i*length);
        }

        paint.setPen(Qt::NoPen);

        for(int i=0;i<x;i++){
           if(currentRect==i){
                paint.setPen(QColor(30,30,50));
                paint.drawText(XMIN + i*(XMAX-XMIN)/x,YMAX-(YMAX-YMIN)*diameterfre[index][i]-10,
                          QString::number(diameterfre[index][i]));
                paint.setPen(Qt::NoPen);
                paint.setBrush(QColor(30,30,50));
            }
            else{
                paint.setBrush(colorVector[i%colorVector.size()]);
            }
            paint.drawRect(XMIN + i*(XMAX-XMIN)/x,YMAX-(YMAX-YMIN)*diameterfre[index][i],(XMAX-XMIN)/x - 1,(YMAX-YMIN)*diameterfre[index][i]);
        }
    }
    else if(isInverse) {
        qDebug()<<"inverse";
        for(int i=0;i<x;i++){
            Rect r(XMIN + i*(XMAX-XMIN)/x,YMAX-(YMAX-YMIN)*idiameterfre[index][i],(XMAX-XMIN)/x - 1,(YMAX-YMIN)*idiameterfre[index][i]);
            rectVector[i] = r;
        }
        float step = 0.1f;
        float length;
        if(num!=0)length = (YMAX - YMIN)/num;
        else length = 0;
        QPainter paint(this);
        paint.setPen(Qt::NoPen);
        paint.setBrush(QColor(255,255,255));
        paint.drawRect(0,0,SIZEW,SIZEH);
        paint.setPen(QColor(0,0,0));
        paint.drawText(XMAX/2,24,QStringLiteral("粒径分布直方图"));
        paint.drawText(XMAX/2,YMAX + 20,QStringLiteral("粒径区间/um"));
        paint.drawText(40,YMAX + 20,QStringLiteral("频率"));
        paint.drawLine(XMIN,YMIN,XMIN,YMAX);
        paint.drawLine(XMIN,YMAX,XMAX,YMAX);
        paint.drawText(20,YMAX + 20,QString::number(0));
        for(int i = 1;i<=num;i++){
            paint.drawText(20,YMAX-i*length,QString::number(step*(i)));
        }
        double s = (idiametermax[index] - idiametermin[index])/(x);
        for(int i=0;i<=x;i++){
            paint.drawText(XMIN + i*(XMAX-XMIN)/x,YMAX+10,QString::number((int)(idiametermin[index]+i*s)));
        }

        paint.setPen(QColor(100,100,100));
        for(int i = 1;i<=num;i++){
            paint.drawLine(XMIN,YMAX-i*length,XMAX,YMAX-i*length);
        }

        paint.setPen(Qt::NoPen);

        for(int i=0;i<x;i++){
           if(currentRect==i){
                paint.setPen(QColor(30,30,50));
                paint.drawText(XMIN + i*(XMAX-XMIN)/x,YMAX-(YMAX-YMIN)*idiameterfre[index][i]-10,
                          QString::number(idiameterfre[index][i]));
                paint.setPen(Qt::NoPen);
                paint.setBrush(QColor(30,30,50));
            }
            else{
                paint.setBrush(colorVector[i%colorVector.size()]);
            }
            paint.drawRect(XMIN + i*(XMAX-XMIN)/x,YMAX-(YMAX-YMIN)*idiameterfre[index][i],(XMAX-XMIN)/x - 1,(YMAX-YMIN)*idiameterfre[index][i]);
        }
    }
    if(isFirstDraw){
        isFirstDraw = false;
    }
}

void plotwindow::saveImg(QString filename,int index)
{
    qDebug()<<"saveIMG";
    int _XMIN = 40,_XMAX = 500,_YMIN = 40,_YMAX = 400,_SIZEW = 580,_SIZEH = 480;
    QPixmap* pixmap = new QPixmap(_SIZEW,_SIZEH);

    QPainter paint(pixmap);
    float step = 0.1f;
    float length;
    if(num!=0)length = (_YMAX - _YMIN)/num;
    else length = 0;

    paint.setPen(Qt::NoPen);
    paint.setBrush(QColor(255,255,255));
    paint.drawRect(0,0,_SIZEW,_SIZEH);
    paint.setPen(QColor(0,0,0));
    paint.drawText(_XMAX/2,24,QStringLiteral("粒径分布直方图"));
    paint.drawText(_XMAX/2,_YMAX + 20,QStringLiteral("粒径区间/um"));
    paint.drawText(40,_YMAX + 20,QStringLiteral("频率"));
    paint.drawLine(_XMIN,_YMIN,_XMIN,_YMAX);
    paint.drawLine(_XMIN,_YMAX,_XMAX,_YMAX);
    paint.drawText(20,_YMAX + 20,QString::number(0));
    for(int i = 1;i<=num;i++){
        paint.drawText(20,_YMAX-i*length,QString::number(step*(i)));
    }
    if(!isInverse) {
        double s = (diametermax[index] - diametermin[index])/(x);
        for(int i=0;i<x;i++){
            paint.drawText(_XMIN + i*(_XMAX-_XMIN)/x,_YMAX+10,QString::number((int)(diametermin[index]+i*s)));
        }

         paint.setPen(QColor(100,100,100));
        for(int i = 1;i<=num;i++){
            paint.drawLine(_XMIN,_YMAX-i*length,_XMAX,_YMAX-i*length);
        }

        paint.setPen(Qt::NoPen);

        for(int i=0;i<x;i++){
            if(currentRect==i){
                paint.setPen(QColor(30,30,50));
                paint.drawText(_XMIN + i*(_XMAX-_XMIN)/x,_YMAX-(_YMAX-_YMIN)*diameterfre[index][i]-10,
                              QString::number(diameterfre[index][i]));
                paint.setPen(Qt::NoPen);
                paint.setBrush(QColor(30,30,50));
            }
            else{
                paint.setBrush(colorVector[i%colorVector.size()]);
            }
            paint.drawRect(_XMIN + i*(_XMAX-_XMIN)/x,_YMAX-(_YMAX-_YMIN)*diameterfre[index][i],(_XMAX-_XMIN)/x - 1,(_YMAX-_YMIN)*diameterfre[index][i]);
        }
        pixmap->save(filename);
    }
    else {
        double s = (idiametermax[index] - idiametermin[index])/(x);
        for(int i=0;i<x;i++){
            paint.drawText(_XMIN + i*(_XMAX-_XMIN)/x,_YMAX+10,QString::number((int)(idiametermin[index]+i*s)));
        }

         paint.setPen(QColor(100,100,100));
        for(int i = 1;i<=num;i++){
            paint.drawLine(_XMIN,_YMAX-i*length,_XMAX,_YMAX-i*length);
        }

        paint.setPen(Qt::NoPen);

        for(int i=0;i<x;i++){
            if(currentRect==i){
                paint.setPen(QColor(30,30,50));
                paint.drawText(_XMIN + i*(_XMAX-_XMIN)/x,_YMAX-(_YMAX-_YMIN)*idiameterfre[index][i]-10,
                              QString::number(idiameterfre[index][i]));
                paint.setPen(Qt::NoPen);
                paint.setBrush(QColor(30,30,50));
            }
            else{
                paint.setBrush(colorVector[i%colorVector.size()]);
            }
            paint.drawRect(_XMIN + i*(_XMAX-_XMIN)/x,_YMAX-(_YMAX-_YMIN)*idiameterfre[index][i],(_XMAX-_XMIN)/x - 1,(_YMAX-_YMIN)*idiameterfre[index][i]);
        }
        pixmap->save(filename);
    }
}

void plotwindow::saveExcel(QString filepath,int index)
{
    qDebug()<<"saveExcel";
    QString Begin = QString::fromLocal8Bit("<html><head></head><body><table border=\"1\" >");
    QString end = QString::fromLocal8Bit("</table></body></html>");
    QList<QString> list;

    QString header = "<tr>";
    header += QString("<td>%1</td>").arg("id");
    header += QString("<td>%1</td>").arg("X");
    header += QString("<td>%1</td>").arg("Y");
    header += QString("<td>%1</td>").arg("Z");
    header += QString("<td>%1</td>").arg(QStringLiteral("颗粒直径"));
    header += QString("<td>%1</td>").arg(QStringLiteral("颗粒总数"));
    header += "</tr>";
    list.push_back(header);

    for(int i=0;i<pointnum[index];i++) {
        QString rowStr = "<tr>";
      //  for(int j=0;j<col;j++) {
            QString cel;
            cel = QString::number(i+1);
            rowStr += QString("<td>%1</td>").arg(cel);
            cel = QString::number(pos[index][i].X);
            rowStr += QString("<td>%1</td>").arg(cel);
            cel = QString::number(pos[index][i].Y);
            rowStr += QString("<td>%1</td>").arg(cel);
            cel = QString::number(pos[index][i].Z);
            rowStr += QString("<td>%1</td>").arg(cel);
            cel = QString::number(pos[index][i].D);
            rowStr += QString("<td>%1</td>").arg(cel);
            if(i == 0) {
                cel = QString::number(pointnum[index]);
                rowStr += QString("<td>%1</td>").arg(cel);
            }
      //  }
        rowStr += "</tr>";
        list.push_back(rowStr);
    }
    QString text = Begin;
    for(int i=0;i<list.size();++i){
        text.append(list.at(i));
    }
    text.append(end);
    QTextEdit textEdit;
    textEdit.setText(text);

    QFile file(filepath);
    if(file.open(QFile::WriteOnly | QIODevice::Text)) {
        QTextStream ts(&file);
        ts.setCodec("UTF-8");
        ts<<textEdit.document()->toHtml("UTF-8");
    }
}

void plotwindow::saveAsImage()
{

    QString filename = QFileDialog::getSaveFileName(this, tr("保存图片"),QDir::currentPath(), tr("Images (*.png);; Images(*.bmp);; Images(*.jpg);"));
    if(filename.isEmpty()) {
        return;
    }
    saveImg(filename,index);
}
