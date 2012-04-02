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

    Session() : layout(Session::SINGLE) {}
    Session(const QString &name, Layout layout, const QString &common_command,
            QStringList commands) :
        name(name), layout(layout), common_command(common_command),
        commands(commands) {}
    ~Session() {}

    QString name;
    Layout layout;
    QString common_command;
    QStringList commands;
};

#endif // SESSION_H
