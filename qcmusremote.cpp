#include "qcmusremote.h"
#include <QStringList>

QCmusRemote::QCmusRemote(QObject *parent) :
    QObject(parent)
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

bool QCmusRemote::connect(const QString& path)
{
    return cmus1.connect(path) && cmus2.connect(path);
}

bool QCmusRemote::connect(const QHostAddress& address,
                          quint16 port,
                          const QString& pwd)
{
    return cmus1.connect(address, port, pwd) && cmus2.connect(address, port, pwd);
}

bool QCmusRemote::connect()
{
    return cmus1.connect() && cmus2.connect();
}

void QCmusRemote::setRefreshInterval(unsigned int msec)
{
    upTimer.setInterval(msec);
    upTimer.start();
}

bool QCmusRemote::update()
{
    QMap<QString, QString> newState;

    if (!cmus1.isConnected())
        return false;

    QString ret(cmus1.sendCmd_sync("status"));

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

    updatePlaylist();

    return true;
}

void QCmusRemote::updatePlaylist()
{
    QList<QSharedPointer<QCmusSong> > playlist;

    QString cmus_playlist = QString(cmus2.dataReturned());

    QStringList songs = cmus_playlist.split("file ");
    foreach(QString song, songs)
        playlist.append(QSharedPointer<QCmusSong>(new QCmusSong("file " + song)));

    emit playlistUpdated(playlist);

    cmus2.sendCmd_async("save -p -e -");
}

void QCmusRemote::toogleRepeat()
{
    cmus1.sendCmd_sync("toggle repeat");
}

void QCmusRemote::tootleShuffle()
{
    cmus1.sendCmd_sync("toggle repeat");
}

void QCmusRemote::stop()
{
    cmus1.sendCmd_sync("player-stop");
}

void QCmusRemote::next()
{
    cmus1.sendCmd_sync("player-next");
}

void QCmusRemote::prev()
{
    cmus1.sendCmd_sync("player-prev");
}

void QCmusRemote::play()
{
    cmus1.sendCmd_sync("player-play");
}

void QCmusRemote::pause()
{
    cmus1.sendCmd_sync("player-pause");
}

void QCmusRemote::playFile(const QString& file)
{
    cmus1.sendCmd_sync(QString("player-play %1").arg(file));
}

void QCmusRemote::setVolume(int vol)
{
    cmus1.sendCmd_sync(QString("vol %1").arg(vol*255/100));
}

void QCmusRemote::setPosition(int pos)
{
    cmus1.sendCmd_sync(QString("seek %1").arg(pos));
}
