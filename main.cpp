#include <QtGui/QApplication>
#include <QtGui/QSystemTrayIcon>
#include <QtGui/QMessageBox>

#include "mainwindow.h"
#include "mainwindow2.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("Qtmus");

    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
        QMessageBox::critical(0, QObject::tr("Systray"),
                              QObject::tr("No system tray detected on this system."));
        QApplication::setQuitOnLastWindowClosed(true);
    }
	// TODO : uncomment this line when ready
    //QApplication::setQuitOnLastWindowClosed(false);

    MainWindow w;
	w.showMaximized();

    return app.exec();
}
