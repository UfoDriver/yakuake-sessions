#ifndef SESSION_H
#define SESSION_H

#include <QStringList>


class Session
{
public:
    enum Layout {
        SINGLE,
        VERTICAL,
        HORIZONTAL,
        QUAD
    };

    Session() : layout(Session::SINGLE) {
        qDebug("Session constructor {NONE}");
    }
    Session(const QString &name, Layout layout, const QString &common_command,
            QStringList commands) :
        name(name), layout(layout), common_command(common_command),
        commands(commands) {
    qDebug("Session constructor [%s]", name.toLocal8Bit().data());
    }
    ~Session() { qDebug("Session destructor [%s]", name.toLocal8Bit().data()); }

    QString name;
    Layout layout;
    QString common_command;
    QStringList commands;
};

#endif // SESSION_H
