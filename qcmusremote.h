#ifndef QCMUSREMOTE_H
#define QCMUSREMOTE_H

#include <QObject>
#include <QHostAddress>
#include <QTimer>
#include <QMap>
#include <QSharedPointer>

#include "qcmusconnection.h"
#include "qcmussong.h"

class QCmusRemote : public QObject
{
    Q_OBJECT
public:
    enum CMusState { STOPPED, PAUSED, PLAYING };

private:
    QTimer upTimer;
    QCmusConnection cmus1, cmus2;
    CMusState curState;
    QMap<QString, QString> status;

    void statusUpdated(QMap<QString, QString> newState);

public:
    explicit QCmusRemote(QObject *parent = 0);

    /* Playlist */
    void updatePlaylist();

    /* Current state */
    CMusState state() const { return curState; }
    unsigned int refreshInterval() const { return upTimer.interval(); }
    const QString file() const { return status["file"]; }
    unsigned int duration() const { return status["duration"].toInt(); }
    unsigned int position() const { return status["position"].toInt(); }
    const QString artist() const { return status["artist"]; }
    const QString album() const { return status["album"]; }
    const QString title() const { return status["title"]; }
    const QString date() const { return status["date"]; }
    const QString genre() const { return status["genre"]; }
    const QString trackNumber() const { return status["tracknumber"]; }
    const QString aaaMode() const { return status["aaa_mode"]; }
    const QString albumArtist() const { return status["albumartist"]; }
    bool continuePlaying() const { return status["continue"]=="true"; }
    bool playsLibrary() const { return status["play_library"]=="true"; }
    bool replayGain() const { return status["replaygain"]!="disabled"; }
    bool replayGainLimit() const { return status["replaygain_limit"]=="true"; }
    const QString replayGainPreAmp() const { return status["replaygain_preamp"]; }
    bool repeat() const { return status["repeat"]=="true"; }
    bool repeatCurrent() const { return status["repeat_current"]=="true"; }
    bool shuffle() const { return status["shuffle"]=="true"; }
    bool softvol() const { return status["softvol"]=="true"; }
    unsigned int volumeRight() const { return status["vol_right"].toInt(); }
    unsigned int volumeLeft() const { return status["vol_left"].toInt(); }

signals:
    void statusUpdated();
    void playlistUpdated(QList<QSharedPointer<QCmusSong> > playlist);

    /* Helpers */
    void stateChanged(QCmusRemote::CMusState curState);
    void positionUpdated(int pos);
    void volumeUpdated(int level);
    void newSongPlayed(const QString& artist,
                       const QString& album,
                       const QString& title,
                       const QString& file,
                       unsigned int duration);

public slots:
    bool connect(const QString& path);
    bool connect(const QHostAddress& address, quint16 port, const QString& pwd = QString());
    bool connect();

    void setRefreshInterval(unsigned int msec);
    bool update();

    void toogleRepeat();
    void tootleShuffle();
    void stop();
    void next();
    void prev();
    void play();
    void pause();
    void playFile(const QString& file);
    void setVolume(int vol);
    void setPosition(int pos);
};

#endif // QCMUSREMOTE_H
