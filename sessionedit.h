#ifndef SESSIONEDIT_H
#define SESSIONEDIT_H

#include <QDialog>
#include "session.h"

namespace Ui {
class SessionEdit;
}

class SessionEdit : public QDialog
{
    Q_OBJECT

public:
    explicit SessionEdit(QWidget *parent, Session *session, bool creationDialog = true);
    ~SessionEdit();
    Session *session;

public slots:
    void sessionNameChanged(const QString &name);
    void sessionTypeChanged();
    void deleteSession();
    void accept();

private:
    Ui::SessionEdit *ui;
    bool creationDialog;
};

#endif // SESSIONEDIT_H
