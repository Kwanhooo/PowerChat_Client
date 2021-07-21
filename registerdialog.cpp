#include "registerdialog.h"
#include "ui_registerdialog.h"


extern QString loginCertificate;

RegisterDialog::RegisterDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RegisterDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("PowerChat 注册");
    this->setWindowFlag(Qt::FramelessWindowHint);
    ui->titleBarGroup->setAlignment(Qt::AlignRight);

    ui->pushButton_reg->setEnabled(false);//开始时注册按钮设置不可用
    this->setFixedSize(this->width(),this->height());//不让用户拉伸窗口大小

    connect(ui->btn_1,SIGNAL(clicked()),this,SLOT(on_btn_1_clicked()));
    connect(ui->btn_2,SIGNAL(clicked()),this,SLOT(on_btn_2_clicked()));
    connect(ui->btn_3,SIGNAL(clicked()),this,SLOT(on_btn_3_clicked()));
    connect(ui->btn_4,SIGNAL(clicked()),this,SLOT(on_btn_4_clicked()));
    connect(ui->btn_5,SIGNAL(clicked()),this,SLOT(on_btn_5_clicked()));
    connect(ui->btn_6,SIGNAL(clicked()),this,SLOT(on_btn_6_clicked()));
    connect(ui->btn_7,SIGNAL(clicked()),this,SLOT(on_btn_7_clicked()));
    connect(ui->btn_8,SIGNAL(clicked()),this,SLOT(on_btn_8_clicked()));
    connect(ui->btn_9,SIGNAL(clicked()),this,SLOT(on_btn_9_clicked()));
    connect(ui->btn_10,SIGNAL(clicked()),this,SLOT(on_btn_10_clicked()));
    connect(ui->btn_11,SIGNAL(clicked()),this,SLOT(on_btn_11_clicked()));
    connect(ui->btn_12,SIGNAL(clicked()),this,SLOT(on_btn_12_clicked()));
    connect(ui->btn_13,SIGNAL(clicked()),this,SLOT(on_btn_13_clicked()));
    connect(ui->btn_14,SIGNAL(clicked()),this,SLOT(on_btn_14_clicked()));
    connect(ui->btn_15,SIGNAL(clicked()),this,SLOT(on_btn_15_clicked()));

    this->avatarSetting = "1";
    ui->label_avatarStatus->setAlignment(Qt::AlignCenter);
    ui->label_avatarStatus->setText("您选择了第 1 个头像");
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
    qDebug()<<"头像编号为："<< this->avatarSetting <<endl;
    if(ui->lineEdit_email->text().trimmed().isEmpty()||ui->lineEdit_phone->text().trimmed().isEmpty()||ui->lineEdit_password->text().trimmed().isEmpty()||ui->lineEdit_userName->text().trimmed().isEmpty())
    {
        QMessageBox::information(this,tr("信息不完整"),tr("您尚有信息为填写完整，请检查后再次提交"));
        return;
    }
    else
    {
        if(ui->lineEdit_userName->text().contains("#")||ui->lineEdit_password->text().contains("#")||
                ui->lineEdit_email->text().contains("#")||ui->lineEdit_phone->text().contains("#"))
        {
            QMessageBox::information(this,"信息不合法","您的输入中包含了非法字符\"#\"，请修改后重新提交！");
            return;
        }

        if(ui->lineEdit->text().trimmed().isEmpty())
            this->Bio = "这个人很懒，什么都没有写";
        else
        {
            this->Bio = ui->lineEdit->text().trimmed();
        }
        loginCertificate = QString("##REGISTER_CERTIFICATE##%1##%2##%3##%4##%5##%6").arg(ui->lineEdit_userName->text()).arg(ui->lineEdit_password->text()).arg(ui->lineEdit_email->text()).arg(ui->lineEdit_phone->text()).arg(avatarSetting).arg(Bio);
        this->close();
    }
}

void RegisterDialog::on_btn_min_clicked()
{
    this->setWindowState(Qt::WindowMinimized);
}

void RegisterDialog::on_btn_close_clicked()
{
    this->close();
    exit(0);
}

void RegisterDialog::on_btn_1_clicked()
{
    avatarSetting = "1";
    ui->label_avatarStatus->setText("您选择了第 1 个头像");
}
void RegisterDialog::on_btn_2_clicked()
{
    avatarSetting = "2";
    ui->label_avatarStatus->setText("您选择了第 2 个头像");
}
void RegisterDialog::on_btn_3_clicked()
{
    avatarSetting = "3";
    ui->label_avatarStatus->setText("您选择了第 3 个头像");
}
void RegisterDialog::on_btn_4_clicked()
{
    avatarSetting = "4";
    ui->label_avatarStatus->setText("您选择了第 4 个头像");
}
void RegisterDialog::on_btn_5_clicked()
{
    avatarSetting = "5";
    ui->label_avatarStatus->setText("您选择了第 5 个头像");
}
void RegisterDialog::on_btn_6_clicked()
{
    avatarSetting = "6";
    ui->label_avatarStatus->setText("您选择了第 6 个头像");
}
void RegisterDialog::on_btn_7_clicked()
{
    avatarSetting = "7";
    ui->label_avatarStatus->setText("您选择了第 7 个头像");
}
void RegisterDialog::on_btn_8_clicked()
{
    avatarSetting = "8";
    ui->label_avatarStatus->setText("您选择了第 8 个头像");
}
void RegisterDialog::on_btn_9_clicked()
{
    avatarSetting = "9";
    ui->label_avatarStatus->setText("您选择了第 9 个头像");
}
void RegisterDialog::on_btn_10_clicked()
{
    avatarSetting = "10";
    ui->label_avatarStatus->setText("您选择了第 10 个头像");
}
void RegisterDialog::on_btn_11_clicked()
{
    avatarSetting = "11";
    ui->label_avatarStatus->setText("您选择了第 11 个头像");
}
void RegisterDialog::on_btn_12_clicked()
{
    avatarSetting = "12";
    ui->label_avatarStatus->setText("您选择了第 12 个头像");
}
void RegisterDialog::on_btn_13_clicked()
{
    avatarSetting = "13";
    ui->label_avatarStatus->setText("您选择了第 13 个头像");
}
void RegisterDialog::on_btn_14_clicked()
{
    avatarSetting = "14";
    ui->label_avatarStatus->setText("您选择了第 14 个头像");
}
void RegisterDialog::on_btn_15_clicked()
{
    avatarSetting = "15";
    ui->label_avatarStatus->setText("您选择了第 15 个头像");
}
