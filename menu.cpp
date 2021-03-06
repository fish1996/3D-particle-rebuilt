﻿#include "menu.h"
#include <QMenu>
#include <QMenuBar>
#include <qDebug>
#include <QAction>
#include <QToolBar>
#include <QVBoxLayout>

menu::menu(QWidget *parent)
    : QWidget(parent)
{
    layout();
}

void menu::layout()
{
    vlayout = new QVBoxLayout();
    menuBar = new QMenuBar();

    fileMenu = new QMenu(QStringLiteral("文件"));
    editMenu = new QMenu(QStringLiteral("设置"));

    menuBar->addMenu(fileMenu);
    menuBar->addMenu(editMenu);

    setAction = new QAction(this);
    saveAction = new QAction(this);
    importAction = new QAction(this);
    clearAction = new QAction(this);
    changeAction1 = new QAction(this);
    startAction = new QAction(this);
    changeAction2 = new QAction(this);
    findCamera = new QAction(this);
    importBackgroundAction = new QAction(this);

    importBackgroundAction->setText(QStringLiteral("&导入背景"));
    changeAction1->setText(QStringLiteral("改变比例尺颜色"));
    changeAction1->setIcon(QIcon(":/image/colorpick1.ico"));
    changeAction2->setText(QStringLiteral("改变测量颜色"));
    changeAction2->setIcon(QIcon(":/image/colorpick2.ico"));
    startAction->setText(QStringLiteral("开启/关闭相机"));
    startAction->setIcon(QIcon(":/image/camera.ico"));
    findCamera->setText(QStringLiteral("查找相机"));
    findCamera->setIcon(QIcon(":/image/search.ico"));

    saveAction = fileMenu->addAction(QIcon(":/image/save.ico"),QStringLiteral("&保存"));
    importAction = fileMenu->addAction(QIcon(":/image/import.ico"),QStringLiteral("&导入原图"));
    fileMenu->addAction(importBackgroundAction);
    setAction = editMenu->addAction(QIcon(":/image/config.ico"),QStringLiteral("&设置"));
    clearAction = editMenu->addAction(QIcon(":/image/editclear.ico"),QStringLiteral("&清除缓存"));

    saveAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_S));
    setAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_K));
    importAction->setShortcut(QKeySequence(Qt::CTRL + Qt:: Key_I));
    importBackgroundAction->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt:: Key_I));

    toolBar = new QToolBar(tr("&文件"));

    toolBar->addSeparator();
    toolBar->addAction(saveAction);
    toolBar->addAction(clearAction);
    toolBar->addAction(setAction);
    toolBar->addAction(importAction);
    toolBar->addAction(changeAction1);
    toolBar->addAction(changeAction2);
    toolBar->addAction(startAction);
    toolBar->addAction(findCamera);

    toolBar->setStyleSheet("background-color:rgb(235,235,235);");


    vlayout->addWidget(menuBar);
    vlayout->addWidget(toolBar);
    vlayout->setSpacing(0);
    vlayout->setMargin(0);

    setLayout(vlayout);
}
