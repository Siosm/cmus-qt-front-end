#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
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
    void newSongPlayed(const QString& artist,
                       const QString& album,
                       const QString& title,
                       const QString& file,
                       unsigned int duration);

private:
    Ui::MainWindow *ui;
    QCmusRemote remote;
};

#endif // MAINWINDOW_H
