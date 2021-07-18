#include "registerdialog.h"
#include "ui_registerdialog.h"

extern QString loginCertificate;

RegisterDialog::RegisterDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RegisterDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("PowerChat 注册");
    ui->pushButton_reg->setEnabled(false);//开始时注册按钮设置不可用
    this->setFixedSize(this->width(),this->height());//不让用户拉伸窗口大小
}

RegisterDialog::~RegisterDialog()
{
    delete ui;
}

void RegisterDialog::on_checkBox_stateChanged(int arg1)//同意用户协议之后允许注册
{
    if(ui->checkBox->isChecked()==true)
        ui->pushButton_reg->setEnabled(true);
    if(ui->checkBox->isChecked()==false)
        ui->pushButton_reg->setEnabled(false);
}

void RegisterDialog::on_pushButton_cancel_clicked()//若取消注册，则关闭注册窗口
{
    this->close();
}

void RegisterDialog::on_pushButton_reg_clicked()
{
    if(ui->lineEdit_email->text().trimmed().isEmpty()||ui->lineEdit_phone->text().trimmed().isEmpty()||ui->lineEdit_password->text().trimmed().isEmpty()||ui->lineEdit_userName->text().trimmed().isEmpty())
        QMessageBox::information(this,tr("信息不完整"),tr("您尚有信息为填写完整，请检查后再次提交"));
    else
    {
        loginCertificate = QString("##REGISTER_CERTIFICATE##%1##%2##%3##%4").arg(ui->lineEdit_userName->text()).arg(ui->lineEdit_password->text()).arg(ui->lineEdit_email->text()).arg(ui->lineEdit_phone->text());
        this->close();
    }
}
