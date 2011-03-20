#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QFileSystemModel>
#include <QTreeView>

#include "qcmusremote.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void cmusStateChanged(QCmusRemote::CMusState curState);
    void newSongPlayed(const QString& artist,
                       const QString& album,
                       const QString& title,
                       const QString& file,
                       unsigned int duration);
    void systemTrayActivated(QSystemTrayIcon::ActivationReason reason);

private:
    Ui::MainWindow *ui;
    QCmusRemote remote;

    // SystemTray
    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;
    QAction *quitAction;

    QMenu *fileMenu;

	QFileSystemModel *browserModel;
};

#endif // MAINWINDOW_H
