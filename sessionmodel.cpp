#include "sessionmodel.h"

SessionModel::SessionModel(const QString &organization, const QString &application) :
    QAbstractListModel()
{
    settings = new QSettings(organization, application);
    QStringList l;
    l << "com1.1" << "com1.2" << "com1.3" << "com1.4";
    sessions["test1"] = Session(Session::VERTICAL, "ls", l);
}


SessionModel::~SessionModel()
{
    delete settings;
}

int SessionModel::rowCount(const QModelIndex &index) const
{
    return sessions.size();
}

QVariant SessionModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole) {
        return sessions.keys().at(index.row());
    }
    return QVariant();
}
