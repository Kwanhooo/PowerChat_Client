#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QDebug>
#include <QMessageBox>

#include "powerchatclient.h"
#include "registerdialog.h"

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QDialog *parent = nullptr);
    ~LoginDialog();

private slots:
    void on_pushButton_login_clicked();
    void on_pushButton_reg_clicked();

private:
    Ui::LoginDialog *ui;
};

#endif // LOGINDIALOG_H
