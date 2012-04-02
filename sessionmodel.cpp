#include <QModelIndex>
#include "sessionmodel.h"

SessionModel::SessionModel(const QString &organization, const QString &application) :
    QAbstractListModel()
{
    settings = new QSettings(organization, application);
    foreach (const QString &key, settings->childGroups()) {
        settings->beginGroup(key);

        sessions << new Session(key, static_cast<Session::Layout>(settings->value("layout", Session::SINGLE).toUInt()),
                                settings->value("common_command").toString(),
                                settings->value("commands").toStringList());
        settings->endGroup();
    }
}


SessionModel::~SessionModel()
{
    settings->clear();
    foreach (const Session *session, sessions) {
        settings->beginGroup(session->name);
        settings->setValue("layout", session->layout);
        settings->setValue("common_command", session->common_command);
        settings->setValue("commands", session->commands);
        settings->endGroup();
        delete session;
    }
    delete settings;
}

int SessionModel::rowCount(const QModelIndex &) const
{
    return sessions.size();
}

QVariant SessionModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole) {
        return sessions.at(index.row())->name;
    }
    return QVariant();
}

void SessionModel::addSession(Session *session)
{
    beginInsertRows(QModelIndex(), sessions.size(), sessions.size() + 1);
    sessions << session;
    endInsertRows();
}

void SessionModel::destroySession(Session *session)
{
    int index = sessions.indexOf(session);
    beginRemoveRows(QModelIndex(), index, index);
    sessions.removeAt(index);
    delete session;
    endRemoveRows();
}
