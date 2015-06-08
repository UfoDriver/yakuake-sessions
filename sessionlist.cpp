#include <QDBusInterface>
#include <QItemSelectionModel>
#include <QListIterator>
#include <QMessageBox>
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
    QDBusInterface yakuake("org.kde.yakuake", "/yakuake/sessions", "org.kde.yakuake");
    QDBusInterface yakuakeTabs("org.kde.yakuake", "/yakuake/tabs", "org.kde.yakuake");
    QDBusInterface yakuakeWin("org.kde.yakuake", "/yakuake/window", "org.kde.yakuake");

    QItemSelectionModel *selectionModel = ui->listView->selectionModel();

    if (yakuake.isValid()) {
        foreach (const QModelIndex index, selectionModel->selectedIndexes()) {
            Session *session = (Session *)model->data(index, Qt::UserRole).value<void *>();
            QVariant sessionId = yakuake.call(SessionList::commandsMap[session->layout]).arguments().takeFirst();
            QStringList terminalIds = yakuake.call("terminalIdsForSessionId", sessionId).arguments().takeFirst().toString().split(",");

            if (session->common_command.size()) {
                foreach (const QString &terminalId, terminalIds) {
                    yakuake.call("runCommandInTerminal", terminalId.toInt(), session->common_command);
                }
            }

            QListIterator<QString> terminals(terminalIds);
            QListIterator<QString> commands(session->commands);
            while (terminals.hasNext() && commands.hasNext()) {
                yakuake.call("runCommandInTerminal", terminals.next().toInt(), commands.next());
            }

            yakuakeTabs.call("setTabTitle", sessionId, session->name);
        }

        yakuakeWin.call("toggleWindowState");

        delete selectionModel;
        qApp->exit();
    } else {
        QMessageBox::warning(this, "Warning", "It seems that Yakuake is not running.");
    }
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

const char* SessionList::commandsMap[] = {"addSession",
                                          "addSessionTwoHorizontal",
                                          "addSessionTwoVertical",
                                          "addSessionQuad"};
