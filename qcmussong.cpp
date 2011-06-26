#include "qcmussong.h"

#include <QStringList>

QCmusSong::QCmusSong(const QString& cmusEntry, QObject *parent) :
    QObject(parent)
{
    if (!cmusEntry.startsWith("file ") || cmusEntry.length() <= 5)
        return;

    QStringList lines = cmusEntry.split("\n");

    foreach(QString line, lines)
    {
        if (line.startsWith("file "))
            _file = line.mid(QString("file ").length());
        else if (line.startsWith("duration "))
            _duration = line.mid(QString("duration ").length()).toInt();
        else if (line.startsWith("tag "))
        {
            int pos = line.indexOf(" ", 4);
            if (pos > 0)
                attributes[line.mid(4, pos-4)] = line.mid(pos+1);
        }
    }
}

const QString QCmusSong::file() const
{
    return _file;
}

const QString QCmusSong::title() const
{
    return attributes["title"];
}

const QString QCmusSong::album() const
{
    return attributes["album"];
}

const QString QCmusSong::artist() const
{
    return attributes["artist"];
}

unsigned int QCmusSong::duration() const
{
    return _duration;
}
