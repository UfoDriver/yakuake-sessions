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

    Session();
    Session(Layout layout, const QString &common_command, QStringList commands);

    Layout layout;
    QString common_command;
    QStringList commands;
};

#endif // SESSION_H
