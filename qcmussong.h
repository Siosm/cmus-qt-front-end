#ifndef QCMUSSONG_H
#define QCMUSSONG_H

#include <QObject>
#include <QString>
#include <QMap>

class QCmusSong : public QObject
{
    Q_OBJECT

    QString _file;
    unsigned int _duration;

    QMap<QString, QString> attributes;
public:
    explicit QCmusSong(const QString& cmusEntry = QString(), QObject *parent = 0);

    const QString file() const;
    const QString title() const;
    const QString album() const;
    const QString artist() const;
    unsigned int duration() const;

signals:

public slots:

};

#endif // QCMUSSONG_H
