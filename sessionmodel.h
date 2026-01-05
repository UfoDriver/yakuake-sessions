#ifndef SESSIONMODEL_H
#define SESSIONMODEL_H

#include <QAbstractListModel>
#include <QSettings>
#include <QList>
#include "session.h"


class SessionModel : public QAbstractListModel
{
    Q_OBJECT
public:
    SessionModel(const QString &organization, const QString &application);
    ~SessionModel();
    int rowCount(const QModelIndex &index) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
//    Session *getSession(int index) { return sessions.at(index); }
    void addSession(Session *session);
    void destroySession(Session *session);
    void setShowHotkeys(bool state)
    {
        m_showHotkeys = state;
        dataChanged(index(0, 0), index(sessions.length() - 1, 1));
    };
    bool showHotkeys() { return m_showHotkeys; };

signals:

public slots:

private:
    QSettings settings;
    QList<Session *> sessions;
    bool m_showHotkeys = false;
};

#endif // SESSIONMODEL_H
