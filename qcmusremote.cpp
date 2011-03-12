#include "qcmusremote.h"
#include <QStringList>

QCmusRemote::QCmusRemote(QObject *parent) :
    QObject(parent), socket(NULL)
{
}

void QCmusRemote::statusUpdated(QMap<QString, QString> newState)
{
    bool updated = false;

    QMapIterator<QString, QString> i(newState);
    while (i.hasNext()) {
        i.next();

        if (status[i.key()] != i.value())
        {
            updated = true;

            if (i.key() == "position")
                emit positionUpdated(i.value().toInt());
            else if (i.key() == "duration")
                emit durationUpdated(i.value().toInt());
            else if (i.key() == "vol_left")
                emit volumeUpdated(i.value().toInt());
        }
        cout << i.key() << ": " << i.value() << endl;
    }

    if (updated)
        emit statusUpdated();
}

QByteArray QCmusRemote::sendCmd(QString cmd)
{
    QByteArray ret;

    if (!socket || cmd == QString())
        return QByteArray();

    if (cmd.right(1) != "\n")
        cmd += "\n";

    qint64 wr_len = socket->write(cmd.toAscii());

    socket->waitForReadyRead(100);

    while (socket->isReadable() && ret.right(2) != "\n\n")
    {
        char buf[999];
        qint64 len = socket->readLine(buf, sizeof(buf));
        ret.append(buf, len);
    }

//qDebug("cmd = '%s', ret = '%s'", qPrintable(cmd), ret.constData());

    return ret;
}

bool QCmusRemote::connect(const QString& path)
{
    localSocket.connectToServer(path);

    if (localSocket.waitForConnected())
    {
        socket = dynamic_cast<QIODevice*>(&localSocket);
        return true;
    }
    else
        return false;
}

bool QCmusRemote::connect(const QHostAddress& address,
                          quint16 port,
                          const QString& pwd)
{
    tcpSocket.connectToHost(address, port);

    if (tcpSocket.waitForConnected())
    {
        socket = dynamic_cast<QIODevice*>(&tcpSocket);
        sendCmd(pwd);
        return true;
    }
    else
        return false;
}

bool QCmusRemote::connect()
{
    QString path = QString("%1/.cmus/socket");
    path = path.arg(getenv("HOME"));

    return connect(path);
}

bool QCmusRemote::update()
{
    QMap<QString, QString> newState;

    if (!socket)
        return false;

    QString ret(sendCmd("status"));

    QStringList lines = ret.split('\n');
    foreach ( const QString &line, lines ) {
        QStringList words = line.split(' ');

        if (words[0] == "set" || words[0] == "tag")
            words.removeFirst();

        QString key = words[0];

        words.removeFirst();
        QString value = words.join(" ");

        newState[key] = value;
    }

    statusUpdated(newState);

    return true;
}

