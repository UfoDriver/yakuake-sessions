#include <QDBusConnection>
#include <QDBusMessage>
#include <QItemSelectionModel>
#include <QListIterator>
#include <QModelIndex>
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

    connect(ui->listView->selectionModel(),
            SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection &)),
            SLOT(selectionChanged()));
}

SessionList::~SessionList()
{
    delete ui;
    delete model;
}

void SessionList::startSession()
{
    QDBusConnection bus = QDBusConnection::sessionBus();
    QItemSelectionModel *selectionModel = ui->listView->selectionModel();

    foreach (const QModelIndex index, selectionModel->selectedIndexes()) {
        Session *session = (Session *)model->data(index, Qt::UserRole).value<void *>();

        switch (session->layout) {
            case Session::QUAD:
                bus.call(QDBusMessage::createMethodCall("org.kde.yakuake", "/yakuake/sessions", "org.kde.yakuake", "addSessionQuad"));
                break;
            case Session::HORIZONTAL:
                bus.call(QDBusMessage::createMethodCall("org.kde.yakuake", "/yakuake/sessions", "org.kde.yakuake", "addSessionTwoVertical"));
                break;
            case Session::VERTICAL:
                bus.call(QDBusMessage::createMethodCall("org.kde.yakuake", "/yakuake/sessions", "org.kde.yakuake", "addSessionTwoHorizontal"));
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
    }

    delete selectionModel;
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
    Session *session = (Session *)model->data(ui->listView->currentIndex(), Qt::UserRole).value<void *>();
    if (SessionEdit(this, session, false).exec() == 2) {
        model->destroySession(session);
    }
}

void SessionList::selectionChanged()
{
    bool isSingle = ui->listView->selectionModel()->selectedIndexes().length() == 1;
    ui->startButton->setEnabled(true);
    ui->editButton->setEnabled(isSingle);
}
