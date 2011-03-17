#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->previousButton->setIcon(style()->standardIcon(QStyle::SP_MediaSkipBackward));
    ui->nextButton->setIcon(style()->standardIcon(QStyle::SP_MediaSkipForward));
    ui->playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));

    connect(&remote, SIGNAL(stateChanged(QCmusRemote::CMusState)), this, SLOT(cmusStateChanged(QCmusRemote::CMusState)));
    connect(&remote, SIGNAL(volumeUpdated(int)), ui->volumeSlider, SLOT(setValue(int)));
    connect(ui->volumeSlider, SIGNAL(sliderMoved(int)), &remote, SLOT(setVolume(int)));
    connect(&remote, SIGNAL(positionUpdated(int)), ui->playingSlider, SLOT(setValue(int)));
    connect(ui->playingSlider, SIGNAL(sliderMoved(int)), &remote, SLOT(setPosition(int)));
    connect(&remote, SIGNAL(newSongPlayed(QString,QString,QString,QString,uint)), this, SLOT(newSongPlayed(QString,QString,QString,QString,uint)));

    connect(ui->previousButton, SIGNAL(clicked()), &remote, SLOT(prev()));
    connect(ui->playButton, SIGNAL(clicked()), &remote, SLOT(pause()));
    connect(ui->nextButton, SIGNAL(clicked()), &remote, SLOT(next()));

    //FIXME : find the right icon for the quit action!
    quitAction = new QAction(style()->standardIcon(QStyle::SP_DialogCloseButton), tr("&Quit"), this);
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));

    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(quitAction);
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayIconMenu);
    trayIcon->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    trayIcon->show();
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(systemTrayActivated(QSystemTrayIcon::ActivationReason)));

    setWindowIcon(style()->standardIcon(QStyle::SP_MediaPlay));

    if (!remote.connect())
        QMessageBox::critical(this, tr("CMus connection Error"), tr("Couldn't connect to CMus. Please check your settings"));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::cmusStateChanged(QCmusRemote::CMusState curState)
{
    if (curState == QCmusRemote::PLAYING)
        ui->playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
    else
        ui->playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
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
    ui->trackNameLabel->setText(title);
}

void MainWindow::systemTrayActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch(reason){
        case QSystemTrayIcon::Trigger:
        case QSystemTrayIcon::DoubleClick:
            if(this->isVisible())
                QMainWindow::setVisible(false);
            else
                QMainWindow::setVisible(true);
            break;
    }
}
