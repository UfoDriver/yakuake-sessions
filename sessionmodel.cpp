#include "sessionmodel.h"
#include <QIcon>
#include <QModelIndex>
#include <algorithm>

SessionModel::SessionModel(const QString &organization,
                           const QString &application)
    : QAbstractListModel(), settings(organization, application) {
  for (const QString &key : settings.childGroups()) {
    settings.beginGroup(key);
    sessions << new Session(
        key,
        static_cast<Session::Layout>(
            settings.value("layout", Session::SINGLE).toUInt()),
        settings.value("common_command").toString(),
        settings.value("commands").toStringList(),
        settings.value("favorite").toBool());
    settings.endGroup();
  }

  std::sort(sessions.begin(), sessions.end(), compare_sessions_by_pointer);
}

SessionModel::~SessionModel() {
  settings.clear();
  for (const Session *session : sessions) {
    settings.beginGroup(session->name);
    settings.setValue("layout", session->layout);
    settings.setValue("common_command", session->common_command);
    settings.setValue("commands", session->commands);
    settings.setValue("favorite", session->favorite);
    settings.endGroup();
    delete session;
  }
}

int SessionModel::rowCount(const QModelIndex &) const {
  return sessions.size();
}

QVariant SessionModel::data(const QModelIndex &index, int role) const {
  switch (role) {
  case Qt::DisplayRole:
    if (m_showHotkeys) {
      return QString::fromUtf8("[%0] %1", -1)
          .arg(index.row() + 1)
          .arg(sessions.at(index.row())->name);
    } else {
      return sessions.at(index.row())->name;
    }
  case Qt::UserRole:
    return QVariant::fromValue((void *)sessions.at(index.row()));
  case Qt::DecorationRole:
    if (sessions.at(index.row())->favorite) {
      return QIcon::fromTheme("favorite");
    } else {
      return QVariant();
    }
  default:
    return QVariant();
  }
}

void SessionModel::addSession(Session *session) {
  beginInsertRows(QModelIndex(), sessions.size(), sessions.size() + 1);
  sessions << session;
  endInsertRows();
}

void SessionModel::destroySession(Session *session) {
  int index = sessions.indexOf(session);
  beginRemoveRows(QModelIndex(), index, index);
  sessions.removeAt(index);
  delete session;
  endRemoveRows();
}
