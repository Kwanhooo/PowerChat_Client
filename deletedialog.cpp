#include "deletedialog.h"
#include "ui_deletedialog.h"

DeleteDialog::DeleteDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DeleteDialog)
{
    ui->setupUi(this);

    this->setWindowTitle("PowerChat 好友删除");
    this->setWindowFlag(Qt::FramelessWindowHint);
    ui->titleBarGroup->setAlignment(Qt::AlignRight);
    this->setFixedSize(this->width(),this->height());
}

DeleteDialog::~DeleteDialog()
{
    delete ui;
}

void DeleteDialog::deleteGetTcpSocket(QString userName,QTcpSocket *tcpSocketParameter)
{
    this->userName = userName;
    this->tcpSocket = tcpSocketParameter;
    qDebug()<<"打开了添加好友界面，收到的socket::"<<endl<<this->tcpSocket->peerAddress().toString();
}



void DeleteDialog::deleteGetResponse(QString response)
//##DELETE_STATUS##FRIENDNAME##STATUS
{
    qDebug()<<"删除好友窗口收到了response::"<<response<<endl;
    QString status = response.section("##",3,3);
    QString friendName = response.section("##",2,2);
    if(status == "SUCCESS")
    {
        QMessageBox::information(this,"删除消息",QString("已经把 %1 删除咯！").arg(friendName));
        ui->lineEdit_name->clear();
        this->hide();
    }
    else
    {
        QMessageBox::information(this,"删除消息",QString("你的好友列表里没有 %1 哟！").arg(friendName));
    }
}

void DeleteDialog::on_btn_search_clicked()
{
    if(ui->lineEdit_name->text().trimmed().isEmpty())
    {
        QMessageBox::information(this,"没输入呢","您还没有输入您要删除的好友呢");
        return;
    }
    QString friendName = ui->lineEdit_name->text().trimmed();

    QMessageBox requestBox;
    requestBox.setWindowTitle("");
    requestBox.setText(QString("您真的要删除 %1 这位老伙计吗？").arg(friendName));
    requestBox.setStandardButtons(QMessageBox::Yes|QMessageBox::No);
    if(requestBox.exec() == QMessageBox::Yes)
    {
        QString deleteCommand = "##DELETE_REQUEST##";
        deleteCommand.append(this->userName);
        deleteCommand.append("##");
        deleteCommand.append(friendName);
        qDebug()<<"向服务器发送删除好友请求::"<<deleteCommand<<endl;
        this->tcpSocket->write(deleteCommand.toUtf8());
        qDebug()<<"发送成功"<<endl;
        //最终指令形式
        //##DELETE_REQUEST##USERNAME##FRIENDNAME
    }
    else
    {
        QMessageBox::information(this,"","果然还是不忍心呢！😄");
    }
}

void DeleteDialog::on_btn_cancel_clicked()
{
    ui->lineEdit_name->clear();
    this->close();
}

void DeleteDialog::on_btn_min_clicked()
{
    this->setWindowState(Qt::WindowMinimized);
}

void DeleteDialog::on_btn_close_clicked()
{
    ui->lineEdit_name->clear();
    this->close();
}

