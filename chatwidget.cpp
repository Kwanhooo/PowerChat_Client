#include "chatwidget.h"
#include "ui_chatwidget.h"

ChatWidget::ChatWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChatWidget)
{
    ui->setupUi(this);
    this->setFixedSize(this->width(),this->height());//不让用户拉伸窗口大小
    this->setWindowTitle("PowerChat Chatting");
    this->setWindowFlag(Qt::FramelessWindowHint);
    ui->label_name->setAlignment(Qt::AlignCenter);
    ui->label_name_mine->setAlignment(Qt::AlignCenter);
    ui->label_name_friend->setAlignment(Qt::AlignCenter);
    ui->label_bio_mine->setAlignment(Qt::AlignCenter);
    ui->label_bio_friend->setAlignment(Qt::AlignCenter);
}

ChatWidget::~ChatWidget()
{
    delete ui;
}

void ChatWidget::setupThisWindow(QString userNamePara,QString userAvatarPara,QString userBioPara
                                 ,QString friendNamePara,QString friendAvatarPara,QString friendBioPara,
                                 QTcpSocket *tcpSocketPara,AddDialog* ad)
{
    this->userName = userNamePara;
    this->userAvatar = userAvatarPara;
    this->userBio = userBioPara;
    this->friendName = friendNamePara;
    this->friendAvatar = friendAvatarPara;
    this->friendBio = friendBioPara;

    this->tcpSocket = tcpSocketPara;
    this->ad = ad;

    ui->label_name->setText(friendName);
    ui->label_name_friend->setText(friendName);
    ui->label_name_mine->setText(userName);

    ui->label_head_mine->setStyleSheet(QString("image: url(:/%1.png);").arg(this->userAvatar));
    ui->label_bio_mine->setText(this->userBio);
    ui->label_head_friend->setStyleSheet(QString("image: url(:/%1.png);").arg(this->friendAvatar));
    ui->label_bio_friend->setText(this->friendBio);
}

void ChatWidget::getChatMsg(QString responsePara)
{
    this->response = responsePara;

    //聊天室消息
    if(response.section("##",1,1) == "GROUP_MESSAGE")
        //##GROUP_MESSAGE##SENDER##MSG##ONLINEAMOUNT
    {
        if(!this->friendName.contains("公共聊天室"))
            return;

        qDebug()<<"是聊天室消息！"<<endl;
        QString nameInResponse = response.section("##",2,2);
        QString msg = response.section("##",3,3);
        qDebug()<<"聊天室收到response::"<<response;
        //显示时间戳
        ui->textBrowser->append("------------------------" + QTime::currentTime().toString("hh:mm:ss") + "-------------------------");
        //显示聊天室消息
        ui->textBrowser->append(QString("<font color=orange>%1:</font>%2").arg(nameInResponse).arg(msg));
    }
    //好友消息
    else
    {
        qDebug()<<"是好友消息"<<endl;
        //sender##MSG##recipient
        QString nameInResponse = response.section("##",0,0);
        QString msg = response.section("##",1,1);
        if(nameInResponse != this->friendName)
            return;
        qDebug()<<"好友的聊天窗口收到response::"<<response;
        //显示时间戳
        ui->textBrowser->append("------------------------" + QTime::currentTime().toString("hh:mm:ss") + "-------------------------");
        //显示好友消息
        ui->textBrowser->append(QString("<font color=orange>%1:</font>%2").arg(nameInResponse).arg(msg));
    }
}

/*
 * 以下代码段为隐藏标题栏之后（太丑），重写的鼠标事件
 */
void ChatWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_Drag = true;
        m_DragPosition = event->globalPos() - this->pos();
        event->accept();
    }
    QWidget::mousePressEvent(event);
}

void ChatWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (m_Drag && (event->buttons() && Qt::LeftButton))
    {
        move(event->globalPos() - m_DragPosition);
        event->accept();
        emit mouseButtonMove(event->globalPos() - m_DragPosition);
        emit signalMainWindowMove();
    }
    QWidget::mouseMoveEvent(event);
}

void ChatWidget::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    m_Drag = false;
    QWidget::mouseReleaseEvent(event);
}

void ChatWidget::on_btn_min_clicked()
{
    this->setWindowState(Qt::WindowMinimized);
}

void ChatWidget::on_btn_close_clicked()
{
    ui->textEdit->clear();
    this->hide();
}

void ChatWidget::on_btn_close_2_clicked()
{
    ui->textEdit->clear();
    this->hide();
}


void ChatWidget::on_btn_clear_clicked()
{
    ui->textEdit->clear();
}

void ChatWidget::on_btn_addFriends_clicked()
{
    ad->show();
}

void ChatWidget::on_btn_send_clicked()
{
    QString msg = ui->textEdit->toPlainText().trimmed();
    if(msg.isEmpty())
        return;

    if(friendName.contains("公共聊天室"))
    {
        QString senderName = userName;
        QString msgToSend = "##GROUP_MESSAGE##";
        msgToSend.append(userName);
        msgToSend.append("##");
        msgToSend.append(msg);
        tcpSocket->write(msgToSend.toUtf8());
        qDebug()<<"MESSAGE HAS SENT TO SERVER::"<< msgToSend <<endl;
    }
    else
    {
        QString senderName = userName;
        QString recipientName = this->friendName;
        QString msgToSend = senderName.append(QString("##%1##").arg(msg).append(recipientName));

        tcpSocket->write(msgToSend.toUtf8());
        qDebug()<<"MESSAGE HAS SENT TO SERVER::"<< msgToSend <<endl;
    }

    QString msgFormatted = QString("<font color=blue>%1:</font><font color=black>%2</font>")
            .arg(userName).arg(msg);
    ui->textBrowser->append("------------------------" + QTime::currentTime().toString("hh:mm:ss") + "-------------------------");
    ui->textBrowser->append(msgFormatted);//将自己发的消息显示上来
    ui->textEdit->clear();
}
