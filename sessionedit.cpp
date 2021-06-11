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
        ui->singleSessionButton->animateClick();
        ui->saveButton->setDisabled(true);
        ui->saveButton->setText("Create");
        ui->deleteButton->setDisabled(true);
    } else {
        ui->sessionNameEdit->setText(session->name);

        switch (session->layout) {
        case Session::QUAD:
            ui->quadSessionButton->animateClick();
            break;
        case Session::VERTICAL:
            ui->verticalSessionButton->animateClick();
            break;
        case Session::HORIZONTAL:
            ui->horizontalSessionButton->animateClick();
            break;
        default:
            ui->singleSessionButton->animateClick();
        }

        ui->commonCommandEdit->setText(session->common_command);

        for (int i = 0; i < 4; i++) {
            QLineEdit *editor = findChild<QLineEdit *>(QString("command%1Edit").arg(i));
            editor->setText(session->commands.value(i));
        }
        ui->favoriteCheckBox->setCheckState(session->favorite ? Qt::CheckState::Checked : Qt::CheckState::Unchecked);
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

void SessionEdit::sessionTypeChanged()
{
    int enabledItemsCount = 2;

    if (sender() == ui->singleSessionButton) {
        enabledItemsCount = 1;
    } else if (sender() == ui->quadSessionButton) {
        enabledItemsCount = 4;
    }

    for (int i = 0; i < 4; i++) {
        QWidget *editor = findChild<QWidget *>(QString("command%1Edit").arg(i));
        editor->setEnabled(i < enabledItemsCount);
        QWidget *label = findChild<QWidget *>(QString("labelCommand%1").arg(i));
        label->setEnabled(i < enabledItemsCount);
    }
}

void SessionEdit::deleteSession()
{
    done(2);
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
    session->favorite = ui->favoriteCheckBox->checkState() == Qt::CheckState::Checked;

    for (int i = 0; i < 4; i++) {
        QLineEdit *editor = findChild<QLineEdit *>(QString("command%1Edit").arg(i));
        session->commands << editor->text();
    }

    QDialog::accept();
}
