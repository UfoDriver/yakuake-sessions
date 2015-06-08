#ifndef SESSIONLIST_H
#define SESSIONLIST_H

#include <QDialog>
#include <QItemSelection>
#include <QSettings>
#include "session.h"
#include "sessionmodel.h"


namespace Ui {
    class SessionList;
}

class SessionList : public QDialog
{
    Q_OBJECT

public:
    explicit SessionList(QDialog *parent = 0);
    ~SessionList();

public slots:
    void startSession();
    void newSession();
    void editSession();
    void selectionChanged();

private:
    Ui::SessionList *ui;
    SessionModel *model;
    const static char* commandsMap[];
};

#endif // SESSIONLIST_H
