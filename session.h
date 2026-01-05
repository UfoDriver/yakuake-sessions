#ifndef SESSION_H
#define SESSION_H

#include <QStringList>

class Session {
public:
  enum Layout { SINGLE, VERTICAL, HORIZONTAL, QUAD };

  Session() : layout(Session::SINGLE) {}
  Session(const QString &name, Layout layout, const QString &common_command,
          QStringList commands, bool favorite)
      : name(name), layout(layout), common_command(common_command),
        commands(commands), favorite(favorite) {}
  ~Session() {}

  bool operator<(const Session &rhs) const {
    if (favorite ^ rhs.favorite) {
      return favorite;
    } else {
      return name < rhs.name;
    }
  }

  QString name;
  Layout layout;
  QString common_command;
  QStringList commands;
  bool favorite;
};

#endif // SESSION_H
