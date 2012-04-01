#include "session.h"

Session::Session()
{
   layout = Session::SINGLE;
}

Session::Session(Layout _layout, const QString &_common_command, QStringList _commands)
{
    layout = _layout;
    common_command = _common_command;
    commands = _commands;
}
