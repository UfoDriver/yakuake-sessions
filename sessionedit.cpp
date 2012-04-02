#include <QDebug>
#include <QString>
#include "sessionedit.h"
#include "ui_sessionedit.h"


SessionEdit::SessionEdit(QWidget *parent, Session *session, bool creationDialog) :
    QDialog(parent), session(session), ui(new Ui::SessionEdit),
    creationDialog(creationDialog)
{
    ui->setupUi(this);

    if (creationDialog) {
        ui->singleSessionButton->setChecked(true);
        ui->saveButton->setDisabled(true);
        ui->saveButton->setText("Create");
    } else {
        ui->sessionNameEdit->setText(session->name);

        switch (session->layout) {
        case Session::QUAD:
            ui->quadSessionButton->setChecked(true);
            break;
        case Session::VERTICAL:
            ui->verticalSessionButton->setChecked(true);
            break;
        case Session::HORIZONTAL:
            ui->horizontalSessionButton->setChecked(true);
            break;
        default:
            ui->singleSessionButton->setChecked(true);
        }

        ui->commonCommandEdit->setText(session->common_command);

        ui->command0Edit->setText(session->commands.value(0));
        ui->command1Edit->setText(session->commands.value(1));
        ui->command2Edit->setText(session->commands.value(2));
        ui->command3Edit->setText(session->commands.value(3));
    }
}

SessionEdit::~SessionEdit()
{
    delete ui;
}


void SessionEdit::sessionNameChanged(const QString &name)
{
    ui->saveButton->setDisabled(name.isEmpty());
}

void SessionEdit::deleteSession()
{

}

void SessionEdit::accept() {
    session->name = ui->sessionNameEdit->text();
    if (ui->quadSessionButton->isChecked()) {
        session->layout = Session::QUAD;
    } else if (ui->verticalSessionButton->isChecked()) {
        session->layout = Session::VERTICAL;
    } else if (ui->horizontalSessionButton->isChecked()) {
        session->layout = Session::HORIZONTAL;
    } else {
        session->layout = Session::SINGLE;
    }
    session->common_command = ui->commonCommandEdit->text();
    session->commands.clear();
    session->commands << ui->command0Edit->text();
    session->commands << ui->command1Edit->text();
    session->commands << ui->command2Edit->text();
    session->commands << ui->command3Edit->text();
    QDialog::accept();
}
