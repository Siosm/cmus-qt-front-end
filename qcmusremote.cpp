#include "qcmusremote.h"
#include <QStringList>

QCmusRemote::QCmusRemote(QObject *parent) :
    QObject(parent), socket(NULL)
{
    QObject::connect(&upTimer, SIGNAL(timeout()), this, SLOT(update()));
    setRefreshInterval(250);
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

            if (i.key() == "status")
            {
                if (i.value() == "playing")
                    curState = PLAYING;
                else if (i.value() == "paused")
                    curState = PAUSED;
                else if (i.value() == "stopped")
                    curState = STOPPED;

                emit stateChanged(curState);
            }
            else if (i.key() == "file")
            {
                emit newSongPlayed(newState["artist"],
                                   newState["album"],
                                   newState["title"],
                                   newState["file"],
                                   newState["duration"].toInt());
            }
            else if (i.key() == "position") /* TODO: Make sure it is never called before newSongPlayed() */
                emit positionUpdated(i.value().toInt());
            else if (i.key() == "vol_left" || i.key() == "vol_right")
                emit volumeUpdated(i.value().toInt());
        }
    }

    status = newState;

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

    while (socket->isReadable() && ret.right(2) != "\n\n" && ret != "\n")
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

void QCmusRemote::setRefreshInterval(unsigned int msec)
{
    upTimer.setInterval(msec);
    upTimer.start();
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

void QCmusRemote::toogleRepeat()
{
    sendCmd("toggle repeat");
}

void QCmusRemote::tootleShuffle()
{
    sendCmd("toggle repeat");
}

void QCmusRemote::stop()
{
    sendCmd("player-stop");
}

void QCmusRemote::next()
{
    sendCmd("player-next");
}

void QCmusRemote::prev()
{
    sendCmd("player-prev");
}

void QCmusRemote::play()
{
    sendCmd("player-play");
}

void QCmusRemote::pause()
{
    sendCmd("player-pause");
}

void QCmusRemote::playFile(const QString& file)
{
    sendCmd(QString("player-play %1").arg(file));
}

void QCmusRemote::setVolume(int vol)
{
    sendCmd(QString("vol %1").arg(vol*255/100));
    qDebug("Set volume %i", vol*255/100);
}

void QCmusRemote::setPosition(int pos)
{
    sendCmd(QString("seek %1").arg(pos));
    qDebug("Set position!");
}
