#include "logindialog.h"
#include "ui_logindialog.h"
#include "registerdialog.h"

extern QString loginCertificate; //连接至服务器后发送的登录凭证

LoginDialog::LoginDialog(QDialog *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    this->setFixedSize(this->width(),this->height());//不让用户拉伸窗口大小
    this->setWindowTitle("PowerChat 登录");
    ui->lineEdit_password->setEchoMode(QLineEdit::Password);//设置LineEdit的掩码模式
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::on_pushButton_login_clicked()//按下登录按钮之后执行的操作
{
    if (ui->lineEdit_account->text().trimmed() != "" && ui->lineEdit_password->text() != "")
    {
        loginCertificate = QString("##LOGIN_CERTIFICATE##%1##%2").arg(ui->lineEdit_account->text()).arg(ui->lineEdit_password->text());
        //##LOGIN_CERTIFICATE##userName##password
        close();
    }
    else
    {
        QMessageBox::information(this, "账号或密码错误", "用户名或密码为空，请检查后重新输入");
    }
}

void LoginDialog::on_pushButton_reg_clicked()//点击注册之后新建一个注册窗口，并将登录窗口关闭
{
    RegisterDialog *regDialog = new RegisterDialog();
    regDialog->exec();
    this->close();
}
