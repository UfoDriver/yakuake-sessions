#ifndef SESSIONMODEL_H
#define SESSIONMODEL_H

#include <QAbstractListModel>
#include <QSettings>
#include <QMap>
#include "session.h"


class SessionModel : public QAbstractListModel
{
    Q_OBJECT
public:
    SessionModel(const QString &organization, const QString &application);
    ~SessionModel();
    int rowCount(const QModelIndex &index) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    Session getSession(const QString &sessionName) { return sessions.value(sessionName); }

signals:

public slots:

private:
    QSettings *settings;
    QMap<QString, Session> sessions;
};

#endif // SESSIONMODEL_H
