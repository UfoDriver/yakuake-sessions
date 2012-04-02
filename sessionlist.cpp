#include <QDBusConnection>
#include <QDBusMessage>
#include <QListIterator>
#include "session.h"
#include "sessionedit.h"
#include "sessionlist.h"
#include "ui_sessionlist.h"


SessionList::SessionList(QDialog *parent) :
    QDialog(parent),
    ui(new Ui::SessionList)
{
    ui->setupUi(this);
    model = new SessionModel("WildPeppers.net", "YakuakeSessions");
    ui->listView->setModel(model);
}

SessionList::~SessionList()
{
    delete ui;
    delete model;
}

void SessionList::startSession()
{
    QDBusConnection bus = QDBusConnection::sessionBus();
    Session *session = model->getSession(ui->listView->currentIndex().row());

    switch (session->layout) {
        case Session::QUAD:
            bus.call(QDBusMessage::createMethodCall("org.kde.yakuake", "/yakuake/sessions", "org.kde.yakuake", "addSessionQuad"));
            break;
        case Session::HORIZONTAL:
            bus.call(QDBusMessage::createMethodCall("org.kde.yakuake", "/yakuake/sessions", "org.kde.yakuake", "addSessionTwoHorizontal"));
            break;
        case Session::VERTICAL:
            bus.call(QDBusMessage::createMethodCall("org.kde.yakuake", "/yakuake/sessions", "org.kde.yakuake", "addSessionTwoVertical"));
            break;
        default:
            bus.call(QDBusMessage::createMethodCall("org.kde.yakuake", "/yakuake/sessions", "org.kde.yakuake", "addSession"));
    }

    QDBusMessage reply = bus.call(QDBusMessage::createMethodCall("org.kde.yakuake", "/yakuake/sessions", "org.kde.yakuake", "activeSessionId"));
    QVariant sessionId = reply.arguments().takeFirst();
    QDBusMessage terminalIdsMessage = QDBusMessage::createMethodCall("org.kde.yakuake", "/yakuake/sessions", "org.kde.yakuake", "terminalIdsForSessionId");
    terminalIdsMessage << sessionId;
    reply = bus.call(terminalIdsMessage);
    // I'd expect a list of ints here but we have a string with comma separated ids
    QStringList terminalIds = reply.arguments().takeFirst().toString().split(",");

    foreach (const QString &terminalId, terminalIds) {
        if (session->common_command.size()) {
            QDBusMessage command = QDBusMessage::createMethodCall("org.kde.yakuake", "/yakuake/sessions", "org.kde.yakuake", "runCommandInTerminal");
            command << terminalId.toInt() << session->common_command;
            bus.call(command);
        }
    }

    QListIterator<QString> terminals(terminalIds);
    QListIterator<QString> commands(session->commands);
    while (terminals.hasNext() && commands.hasNext()) {
        QDBusMessage command = QDBusMessage::createMethodCall("org.kde.yakuake", "/yakuake/sessions", "org.kde.yakuake", "runCommandInTerminal");
        command << terminals.next().toInt() << commands.next();
        bus.call(command);
    }

    QDBusMessage tabTitleMessage = QDBusMessage::createMethodCall("org.kde.yakuake", "/yakuake/tabs", "org.kde.yakuake", "setTabTitle");
    tabTitleMessage << sessionId << session->name;
    bus.call(tabTitleMessage);
    bus.call(QDBusMessage::createMethodCall("org.kde.yakuake", "/yakuake/window", "org.kde.yakuake", "toggleWindowState"));

    qApp->exit();
}

void SessionList::newSession()
{
    Session *session = new Session();
    SessionEdit dialog(this, session);
    if (dialog.exec()) {
        model->addSession(session);
    } else {
        delete session;
    }
}

void SessionList::editSession()
{
    SessionEdit(this, model->getSession(ui->listView->currentIndex().row()), false).exec();
}

void SessionList::listActivated()
{
   ui->startButton->setEnabled(true);
   ui->editButton->setEnabled(true);
}
