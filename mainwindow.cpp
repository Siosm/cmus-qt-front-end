#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

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

    connect(&remote, SIGNAL(volumeUpdated(int)), ui->volumeSlider, SLOT(setValue(int)));
    connect(ui->volumeSlider, SIGNAL(sliderMoved(int)), &remote, SLOT(setVolume(int)));

    connect(&remote, SIGNAL(positionUpdated(int)), ui->playingSlider, SLOT(setValue(int)));
    connect(ui->playingSlider, SIGNAL(sliderMoved(int)), &remote, SLOT(setPosition(int)));

    connect(&remote, SIGNAL(newSongPlayed(QString,QString,QString,QString,uint)), this, SLOT(newSongPlayed(QString,QString,QString,QString,uint)));

    if (!remote.connect())
        QMessageBox::critical(this, tr("CMus connection Error"), tr("Couldn't connect to CMus. Please check your settings"));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::newSongPlayed(const QString& artist,
                   const QString& album,
                   const QString& title,
                   const QString& file,
                   unsigned int duration)
{
    ui->playingSlider->setMaximum(duration);
    ui->artistLabel->setText(artist);
    ui->albumLabel->setText(album);
}
