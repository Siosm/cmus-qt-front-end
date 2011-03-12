#ifndef QCMUSREMOTE_H
#define QCMUSREMOTE_H

#include <QObject>
#include <QTcpSocket>
#include <QLocalSocket>
#include <QHostAddress>
#include <QMap>

class QCmusRemote : public QObject
{
    Q_OBJECT
private:
    QTcpSocket tcpSocket;
    QLocalSocket localSocket;
    QIODevice* socket;

    QMap<QString, QString> status;

    void statusUpdated(QMap<QString, QString> newState);
    QByteArray sendCmd(QString cmd);

public:
    explicit QCmusRemote(QObject *parent = 0);

    bool connect(const QString& path);
    bool connect(const QHostAddress& address, quint16 port, const QString& pwd = QString());
    bool connect();

    /* inline attributes */
    QString file() { return status["file"]; }
    unsigned int duration() { return status["duration"].toInt(); }
    unsigned int position() { return status["position"].toInt(); }
    QString artist() { return status["artist"]; }
    QString album() { return status["album"]; }
    QString title() { return status["title"]; }
    QString date() { return status["date"]; }
    QString genre() { return status["genre"]; }
    QString trackNumber() { return status["tracknumber"]; }
    QString aaaMode() { return status["aaa_mode"]; }
    QString albumArtist() { return status["albumartist"]; }
    bool continuePlaying() { return status["continue"]=="true"; }
    bool playsLibrary() { return status["play_library"]=="true"; }
    bool replayGain() { return status["replaygain"]!="disabled"; }
    bool replayGainLimit() { return status["replaygain_limit"]=="true"; }
    QString replayGainPreAmp() { return status["replaygain_preamp"]; }
    bool repeat() { return status["repeat"]=="true"; }
    bool repeatCurrent() { return status["repeat_current"]=="true"; }
    bool shuffle() { return status["shuffle"]=="true"; }
    bool softvol() { return status["softvol"]=="true"; }
    unsigned int volumeRight() { return status["vol_right"].toInt(); }
    unsigned int volumeLeft() { return status["vol_left"].toInt(); }

signals:
    void statusUpdated();

    /* Helpers */
    void positionUpdated(unsigned int pos);
    void durationUpdated(unsigned int length);
    void volumeUpdated(unsigned int level);


public slots:
    bool update();
};

#endif // QCMUSREMOTE_H
