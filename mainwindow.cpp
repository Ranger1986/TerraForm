#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <QPushButton>
#include <paintwidget.h>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->paint->start();
    connect(ui->meanFilterButton, SIGNAL(pressed()), ui->paint, SLOT(meanFilter()));
    connect(ui->medFilterButton, SIGNAL(pressed()), ui->paint, SLOT(medianFilter()));
    connect(ui->mountainHeight, SIGNAL(valueChanged(int)), ui->paint, SLOT(setMountainHeight(int)));
    connect(ui->MountainWidth, SIGNAL(valueChanged(int)), ui->paint, SLOT(setMountainWidth(int)));
}

MainWindow::~MainWindow()
{
    delete ui;
}
