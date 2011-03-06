#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->previousButton->setIcon(style()->standardIcon(QStyle::SP_MediaSkipBackward));
    ui->previousButton->setEnabled(false);
    //ui->previousButton->

    ui->nextButton->setIcon(style()->standardIcon(QStyle::SP_MediaSkipForward));
    ui->nextButton->setEnabled(false);

//  playIcon = style()->standardIcon(QStyle::SP_MediaPlay);
//  pauseIcon = style()->standardIcon(QStyle::SP_MediaPause);
//  ui->playButton->setIcon(playIcon);
    ui->playButton->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}
