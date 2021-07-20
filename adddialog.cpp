#include "adddialog.h"
#include "ui_adddialog.h"

AddDialog::AddDialog(QDialog *parent) :
    QDialog(parent),
    ui(new Ui::AddDialog)
{
    ui->setupUi(this);
    this->setWindowFlag(Qt::FramelessWindowHint);
    ui->titleBarGroup->setAlignment(Qt::AlignRight);
    this->setFixedSize(this->width(),this->height());

    this->tcpSocket = new QTcpSocket;
}

AddDialog::~AddDialog()
{
    delete ui;
}

void AddDialog::getTcpSocket(QString userName,QTcpSocket *tcpSocketParameter)
{
    this->userName = userName;
    this->tcpSocket = tcpSocketParameter;
    qDebug()<<"打开了添加好友界面，收到的socket::"<<endl<<this->tcpSocket->peerAddress().toString();
}

void AddDialog::getResponse(QString response)
{
    qDebug()<<"好友添加界面收到RESPONSE::"<<response<<endl;
    //##IS_FRIEND_FOUND##FRIENDNAME##STATUS
    if(response.section("##",3,3)=="TRUE")
    {
        qDebug()<<"找到了好友:"<<response.section("##",2,2);
        QMessageBox::information(this,"成功",QString("好友请求已经发送给%1").arg(response.section("##",2,2)));
        ui->lineEdit_name->clear();
    }
    else
    {
        qDebug()<<"没有找到好友:"<<response.section("##",2,2);
        QMessageBox::information(this,"错误",QString("没有找到您输入的用户:%1，请仔细看看有没有输错哟").arg(response.section("##",2,2)));
    }
}

/*
 * 以下代码段为槽函数
 */

//点击取消关闭窗口
void AddDialog::on_btn_cancel_clicked()
{
    ui->lineEdit_name->clear();
    this->close();
}

//当点击搜索时
void AddDialog::on_btn_search_clicked()
{
    if(ui->lineEdit_name->text().trimmed().isEmpty())
    {
        QMessageBox::information(this,"没输入呢","您还没有输入您要添加的好友呢");
        return;
    }
    QString addCommand = "##ADD_REQUEST##";
    QString friendName = ui->lineEdit_name->text();
    addCommand.append(this->userName);
    addCommand.append("##");
    addCommand.append(friendName);
    qDebug()<<"向服务器发送好友请求::"<<addCommand<<endl;
    this->tcpSocket->write(addCommand.toUtf8());
    qDebug()<<"发送成功"<<endl;
    //最终指令形式
    //##ADD_REQUEST##USERNAME##FRIENDNAME
}

void AddDialog::on_btn_min_clicked()
{
    this->setWindowState(Qt::WindowMinimized);
}

void AddDialog::on_btn_close_clicked()
{
    ui->lineEdit_name->clear();
    this->close();
}
