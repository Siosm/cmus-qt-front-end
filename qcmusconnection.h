#ifndef QCMUSCONNECTION_H
#define QCMUSCONNECTION_H

#include <QObject>
#include <QTcpSocket>
#include <QLocalSocket>
#include <QHostAddress>

class QCmusConnection : public QObject
{
    Q_OBJECT

    QTcpSocket tcpSocket;
    QLocalSocket localSocket;
    QIODevice* socket;

    QByteArray data;

public:
    explicit QCmusConnection(QObject *parent = 0);

    bool isConnected() const;

    QByteArray sendCmd_sync(QString cmd);

    QByteArray dataReturned();

public slots:
    bool connect(const QString& path);
    bool connect(const QHostAddress& address, quint16 port, const QString& pwd = QString());
    bool connect();

    void sendCmd_async(QString cmd);

    void close();

private slots:
    void dataArrived();
};

#endif // QCMUSCONNECTION_H
