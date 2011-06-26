#include "qcmusconnection.h"

QCmusConnection::QCmusConnection(QObject *parent) :
    QObject(parent), socket(NULL)
{
}

bool QCmusConnection::isConnected() const
{
    if (!socket)
        return false;

    return socket->isOpen();
}

QByteArray QCmusConnection::dataReturned()
{
    return data;
}

QByteArray QCmusConnection::sendCmd_sync(QString cmd)
{
    QByteArray ret;

    if (!socket || cmd == QString())
        return QByteArray();

    if (cmd.right(1) != "\n")
        cmd += "\n";

    QObject::disconnect(socket, SIGNAL(readyRead()), this, SLOT(dataArrived()));
    qint64 wr_len = socket->write(cmd.toAscii());

    socket->waitForReadyRead(100);

    while (socket->isReadable() && ret.right(2) != "\n\n" && ret != "\n")
    {
        char buf[999];
        qint64 len = socket->readLine(buf, sizeof(buf));
        ret.append(buf, len);
    }

    return ret;
}

void QCmusConnection::sendCmd_async(QString cmd)
{
    if (!socket || cmd == QString())
        return;

    if (cmd.right(1) != "\n")
        cmd += "\n";

    data.clear();
    QObject::connect(socket, SIGNAL(readyRead()), this, SLOT(dataArrived()));
    qint64 wr_len = socket->write(cmd.toAscii());
}

bool QCmusConnection::connect(const QString& path)
{
    if (socket)
        return false;

    localSocket.connectToServer(path);

    if (localSocket.waitForConnected())
    {
        socket = dynamic_cast<QIODevice*>(&localSocket);
        return true;
    }
    else
        return false;
}

bool QCmusConnection::connect(const QHostAddress& address,
                          quint16 port,
                          const QString& pwd)
{
    if (socket)
        return false;

    tcpSocket.connectToHost(address, port);

    if (tcpSocket.waitForConnected())
    {
        socket = dynamic_cast<QIODevice*>(&tcpSocket);
        sendCmd_sync(pwd);
        return true;
    }
    else
        return false;
}

bool QCmusConnection::connect()
{
    QString path = QString("%1/.cmus/socket");
    path = path.arg(getenv("HOME"));

    return connect(path);
}

void QCmusConnection::close()
{
    if (socket)
        socket->close();
    socket = NULL;
}

void QCmusConnection::dataArrived()
{
    data += socket->readAll();
}
