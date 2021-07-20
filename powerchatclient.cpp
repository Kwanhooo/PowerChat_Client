#include "powerchatclient.h"
#include "ui_powerchatclient.h"

#include "logindialog.h"
#include "registerdialog.h"

QString loginCertificate = "";//登陆凭证设为全局变量，以便在登录注册以及主窗口中都可用

/*
 * 以下代码段为构造函数以及析构函数
 */
PowerChatClient::PowerChatClient(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PowerChatClient)
{
    ui->setupUi(this);
    this->setWindowFlag(Qt::FramelessWindowHint);
    ui->titleBarGroup->setAlignment(Qt::AlignRight);

    //初始化一些必要设置参数
    initParameter();

    //输出本机的网络信息（调试用）
    initClientConfig();

    //建立TCP连接
    setupTCP();

    //第一次与服务器连接
    firstConnectWithServer();

    //初始化其他窗口
    this->ad = new AddDialog();
    ad->hide();
    connect(this,SIGNAL(addWithSocket(QString,QTcpSocket*)),ad,SLOT(getTcpSocket(QString,QTcpSocket*)));
    connect(this,SIGNAL(addResponse(QString)),ad,SLOT(getResponse(QString)));
}

PowerChatClient::~PowerChatClient()
{
    delete ui;
}

void PowerChatClient::initParameter()
{
    //初始化用户列表
    for (int i = 0;i < MAX_USERS_AMOUNT;i++)
    {
        userList[i] = nullptr;
    }
    this->userAmount = 0;//用户总数设为0

    this->hasLoadOffline = false;

    timer = new QTimer();//初始化Qt计时器用于检测与服务器的连接是否超时

    //连接信号和槽
    connect(timer, &QTimer::timeout, this, &PowerChatClient::connectFailed);
}

/*
 * 以下代码段为隐藏标题栏之后（太丑），重写的鼠标事件
 */
void PowerChatClient::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_Drag = true;
        m_DragPosition = event->globalPos() - this->pos();
        event->accept();
    }
    QWidget::mousePressEvent(event);
}

void PowerChatClient::mouseMoveEvent(QMouseEvent *event)
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

void PowerChatClient::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    m_Drag = false;
    QWidget::mouseReleaseEvent(event);
}

/*
 * 以下代码段为运行时所需要的函数
 */

void PowerChatClient::firstConnectWithServer()
{
    qDebug()<<"CONNECTING TO SERVER"<<endl;
    QString IP = "120.78.235.195";
    //仅指令收发端口8800       正式服务器端口10086
    quint16 port = 10086;

    ui->lineEdit_IP->setText(IP);
    ui->lineEdit_port->setText(QString("%1").arg(port));

    tcpSocketToServer->connectToHost(QHostAddress(IP),port);
    tcpSocketToServer->write("##LOGIN_REQUEST");
    //##LOGIN_REQUEST

    qDebug()<<"Now Request for Login"<<endl;
    timer->start(1000);
}

void PowerChatClient::initClientConfig()
{
    //在控制台显示客户端网络信息
    QString localHostName = QHostInfo::localHostName();
    QHostInfo localHostInfo = QHostInfo::fromName(localHostName);
    qDebug()<<"Client:localHostName = "<<localHostName<<endl;
    foreach(QHostAddress address,localHostInfo.addresses())
    {
        if(address.protocol()==QAbstractSocket::IPv4Protocol)
            qDebug()<<"Client:Address = "<<address.toString()<<endl;
    }

    this->setWindowTitle("PowerChat");
}

void PowerChatClient::setupTCP()
{
    /*
     * 以下代码段为Client与Server之间的TCP连接
     */
    this->tcpSocketToServer = new QTcpSocket(this);

    //如果与Server端建立连接
    connect(tcpSocketToServer,&QTcpSocket::connected,[=]()
    {
        qDebug()<<"已经连接至服务器"<<endl;
        ui->label_status->setText("已连接至服务器 120.78.235.195:10086");

        tcpSocketToServer->write("##LOGIN_REQUEST");

        ui->comboBox_status->setCurrentIndex(1);

    });

    //如果与Server端断开连接
    connect(tcpSocketToServer,&QTcpSocket::disconnected,[=]()
    {
        ui->label_status->setText("等待连接...");
        //清空好友列表
        ui->label_online_num->setText("0");
        ui->label_offline_num->setText("0");
        ui->comboBox_online->clear();
        ui->comboBox_offline->clear();
        ui->comboBox_online->addItem("未选择");
        ui->comboBox_offline->addItem("未选择");
        loginCertificate.clear();
    });

    //收到Server端发来的消息
    connect(tcpSocketToServer,&QTcpSocket::readyRead,[=]()
    {
        QByteArray bufferArea = tcpSocketToServer->readAll();
        QString response(bufferArea);
        qDebug()<<"RESPONSE::"<<response<<endl;
        if(response.at(0)=='#'&&response.at(1)=='#')//服务器发来的是指令
        {
            //接收到服务器的登录许可，打开登录窗口
            if(response == "##LOGIN_PERMISSION" )
                //##LOGIN_PERMISSION
            {
                timer->stop();
                LoginDialog *ld = new LoginDialog();
                ld->exec();

                if(loginCertificate!="")
                    this->show();
                else
                    this->close();

                qDebug()<<"发送登录凭证::"<<loginCertificate<<endl;
                ui->btn_send->setEnabled(false);

                if(loginCertificate != "")
                {
                    tcpSocketToServer->write(loginCertificate.toUtf8());
                }
            }
            else if(response.section("##",1,1)=="LOGIN_SUCCESS")
                //##LOGIN_SUCCESS##userName
            {
                this->userName = response.section("##",2,2);
                tcpSocketToServer->write(QString("##REQUEST_USER_CONFIG##%1").arg(userName).toUtf8());
                QString userNameLabelText = userName;
                int currentHour = QTime::currentTime().hour();
                if(currentHour >=1 &&currentHour<=6)
                    userNameLabelText.append("，要注意休息哟！");
                if(currentHour >=7&&currentHour<=11)
                    userNameLabelText.append("，早上好");
                if(currentHour>=12&&currentHour<=13)
                    userNameLabelText.append("，干饭时间到！");
                if(currentHour>=14&&currentHour<=17)
                    userNameLabelText.append("，下午好");
                if(currentHour>=18&&currentHour<=22)
                    userNameLabelText.append("，晚上好");
                if(currentHour == 23||currentHour == 0)
                    userNameLabelText.append("，早点睡觉吧");
                ui->label_userName->setText(userNameLabelText);
            }

            else if (response.section("##",1,1)=="REGISTER_SUCCESS")//##REGISTER_SUCCESS##userName
            {
                userName = response.section("##",2,2);
                QMessageBox::information(nullptr,"注册成功",QString("%1，欢迎加入PowerChat！").arg(this->userName));
                //加入问候语
                QString userNameLabelText = userName;
                int currentHour = QTime::currentTime().hour();
                if(currentHour >=1 &&currentHour<=6)
                    userNameLabelText.append("，要注意休息哟！");
                if(currentHour >=7&&currentHour<=11)
                    userNameLabelText.append("，早上好");
                if(currentHour>=12&&currentHour<=13)
                    userNameLabelText.append("，干饭时间到！");
                if(currentHour>=14&&currentHour<=17)
                    userNameLabelText.append("，下午好");
                if(currentHour>=18&&currentHour<=22)
                    userNameLabelText.append("，晚上好");
                if(currentHour == 23||currentHour == 0)
                    userNameLabelText.append("，早点睡觉吧");
                ui->label_userName->setText(userNameLabelText);

                //登陆成功，向Server端发送申请，获取用户列表信息
                tcpSocketToServer->write(QString("##REQUEST_USER_CONFIG##%1").arg(userName).toUtf8());
                //##REQUEST_USER_CONFIG##userName
            }

            else if(response.section("##",1,1)=="LOGIN_FAILED")
                //##LOGIN_FAILED##userName
            {
                QMessageBox::information(nullptr,tr("登陆错误"),tr("账号密码错误"));
                for (int i = 0;i < userAmount;i++)
                {
                    delete userList[i];
                    userList[i]=nullptr;
                }
                userAmount = 0;
                tcpSocketToServer->disconnectFromHost();

                //重新连接并登录
                this->hide();
                this->firstConnectWithServer();
            }

            else if(response.section("##",1,1)=="REGISTER_FAILED")
            {
                QMessageBox::information(nullptr,tr("注册失败"),tr("您注册的用户名已存在！"));
                for (int i = 0;i < userAmount;i++)
                {
                    delete userList[i];
                    userList[i]=nullptr;
                }
                userAmount = 0;
                tcpSocketToServer->disconnectFromHost();

                //重新连接并登录
                this->hide();
                this->firstConnectWithServer();
            }

            else if(response.section("##",1,1)=="UPDATE_USER_CONFIG")//重新刷新一次客户端的用户列表
                //##UPDATE_USER_CONFIG##USERAMOUNT##INFO STREAM
            {
                qDebug()<<"检测到载入用户信息指令"<<endl;

                //置空用户列表
                for (int i = 0;i < this->MAX_USERS_AMOUNT;i++)
                {
                    this->userList[i]=nullptr;
                }
                this->userAmount = 0;

                //将UI界面恢复初始
                QString offlineCurrentPos = ui->comboBox_offline->currentText();
                QString onlineCurrentPos = ui->comboBox_online->currentText();
                ui->comboBox_offline->clear();
                ui->comboBox_online->clear();
                ui->comboBox_offline->addItem("未选择");
                ui->comboBox_online->addItem("未选择");
                qDebug()<<"重置UI成功"<<endl;

                //设置用户信息
                this->userAmount = response.section("##",2,2).toInt();
                qDebug()<<"userAmount = "<<userAmount<<endl;
                if(this->userAmount > 0)
                {
                    QString info = response.section("##",3,3);
                    QTextStream userInfo(&info);
                    int onlineAmount = 0;
                    int offlineAmount = 0;

                    //将userInfo以流的方式放入到客户端的userList中
                    for (int i = 0;i < this->userAmount;i++)
                    {
                        QString name,IP;
                        int port;
                        int status;
                        userInfo>>name;
                        userInfo>>status;
                        userInfo>>IP;
                        userInfo>>port;

                        User* user = new User(name,status,IP,port);
                        this->userList[i] = user;

                        //将用户列表分列到在线和离线中
                        if(status>=1&&status!=6)
                        {
                            if(status==1)
                                ui->comboBox_online->addItem(name.append(" -> 在线"));
                            if(status==2)
                                ui->comboBox_online->addItem(name.append(" -> Q我吧"));
                            if(status==3)
                                ui->comboBox_online->addItem(name.append(" -> 离开"));
                            if(status==4)
                                ui->comboBox_online->addItem(name.append(" -> 忙碌"));
                            if(status==5)
                                ui->comboBox_online->addItem(name.append(" -> 请勿打扰"));
                            onlineAmount++;
                        }
                        else
                        {
                            if(name==userName&&status==6)
                            {
                                ui->comboBox_online->addItem(name.append(" -> 隐身"));
                                onlineAmount++;
                            }
                            else
                            {
                                ui->comboBox_offline->addItem(name);
                                offlineAmount++;
                            }
                        }
                    }
                    ui->label_online_num->setText(QString("%1").arg(onlineAmount));
                    ui->label_offline_num->setText(QString("%1").arg(offlineAmount));
                    ui->comboBox_online->setCurrentText(onlineCurrentPos);
                    ui->comboBox_offline->setCurrentText(offlineCurrentPos);
                }
                else
                {
                    ui->label_online_num->setText("0");
                    ui->label_offline_num->setText("0");
                }

                //更新完用户列表之后请求获取离线消息
                if(!hasLoadOffline)
                {
                    hasLoadOffline = true;
                    tcpSocketToServer->write(QString("##GET_OFFLINE_MESSAGE##%1").arg(this->userName).toUtf8());
                    //##GET_OFFLINE_MESSAGE##USERNAME
                    qDebug()<<"##GET_OFFLINE_MESSAGE"<<endl;
                }
            }
            else if(response.section("##",1,1)=="OFFLINE_MESSAGE")
                //##OFFLINE_MESSAGE##amount##senderName&&msg&&recipientName(MSG1)##(MSG2)##....
            {
                if(response.section("##",2,2).toInt()==0)
                    ui->textBrowser->append("--------您离线期间无好友消息---------");
                else
                    //离线消息
                {
                    qDebug()<<"OFFLINE_MESSAGE == "<<response<<endl;
                    int amount = response.section("##",2,2).toInt();
                    for (int offlineMsgIndex = 3;offlineMsgIndex< amount+3;offlineMsgIndex++)
                    {
                        QString temp = response.section("##",offlineMsgIndex,offlineMsgIndex);
                        QString friendName = temp.section("&&",0,0);
                        QString msg = temp.section("&&",1,1);
                        QString recipientName = temp.section("&&",2,2);
                        qDebug()<<msg<<endl;
                        ui->textBrowser->append("------------您有离线消息-----------");

                        //是指令类型的信息
                        if(msg.contains("NEW_FRIEND_REQUEST"))
                            //@@NEW_FRIEND_REQUEST@@REQUESTER
                        {
                            QString requesterName = msg.section("@@",2,2);
                            QMessageBox requestBox;
                            requestBox.setWindowTitle("验证消息");
                            requestBox.setText(QString("%1请求添加您为好友，是否同意？").arg(requesterName));
                            requestBox.setStandardButtons(QMessageBox::Yes|QMessageBox::No);
                            if(requestBox.exec() == QMessageBox::Yes)
                            {
                                QString responseToServer = "##FRIEND_REQUEST_RETURN##";
                                //##FRIEND_REQUEST_RETURN##REQUESTER##USERNAME##STATUS
                                responseToServer.append(requesterName);
                                responseToServer.append("##");
                                responseToServer.append(userName);
                                responseToServer.append("##");
                                responseToServer.append("ACCEPTED");
                                qDebug()<<QString("已同意来自%1的好友请求，现在向服务器回应::%2").arg(requesterName).arg(responseToServer)<<endl;
                                tcpSocketToServer->write(responseToServer.toUtf8());
                                qDebug()<<"发送回应完成"<<endl;
                            }
                            else
                            {
                                QString responseToServer = "##FRIEND_REQUEST_RETURN##";
                                responseToServer.append(requesterName);
                                responseToServer.append("##");
                                responseToServer.append(userName);
                                responseToServer.append("##");
                                responseToServer.append("REFUSED");
                                qDebug()<<QString("已拒绝来自%1的好友请求，现在向服务器回应::%2").arg(requesterName).arg(responseToServer)<<endl;
                                tcpSocketToServer->write(responseToServer.toUtf8());
                                qDebug()<<"发送回应完成"<<endl;
                            }
                        }

                        else if(msg.contains("FRIEND_REQUEST_STATUS"))
                            //@@FRIEND_REQUEST_STATUS@@FRIENDNAME@@STATUS
                        {
                            QString friendName = msg.section("@@",2,2);
                            QString status = msg.section("@@",3,3);

                            if(status=="ACCEPTED")
                            {
                                QMessageBox::information(this,"验证消息",QString("%1已接受您的好友请求！").arg(friendName));
                                //重新向服务器请求更新一次好友列表
                                tcpSocketToServer->write(QString("##REQUEST_USER_CONFIG##%1").arg(userName).toUtf8());
                            }
                            else
                            {
                                QMessageBox::information(this,"验证消息",QString("%1拒绝了您的好友请求").arg(friendName));
                            }
                        }

                        //是好友消息
                        else
                            ui->textBrowser->append(QString("<font color=orange>%1:</font><font color=black>%2</font>").arg(friendName).arg(msg)); //消息显示格式！
                    }
                }
            }
            else if(response.section("##",1,1) == "IS_FRIEND_FOUND")
                //##IS_FRIEND_FOUND##FRIENDNAME##STATUS
            {
                emit(addResponse(response));
            }
            else if(response.section("##",1,1) == "FRIEND_REQUEST_STATUS")
                //##FRIEND_REQUEST_STATUS##FRIENDNAME##STATUS
            {
                QString friendName = response.section("##",2,2);
                QString status = response.section("##",3,3);
                if(status=="ACCEPTED")
                {
                    QMessageBox::information(this,"验证消息",QString("%1已接受您的好友请求！").arg(friendName));
                    //重新向服务器请求更新一次好友列表
                    tcpSocketToServer->write(QString("##REQUEST_USER_CONFIG##%1").arg(userName).toUtf8());
                }
                else
                {
                    QMessageBox::information(this,"验证消息",QString("%1拒绝了您的好友请求").arg(friendName));
                }
            }
            else if(response.section("##",1,1) == "NEW_FRIEND_REQUEST")
                //##NEW_FRIEND_REQUEST##REQUESTER
            {
                QString requesterName = response.section("##",2,2);
                QMessageBox requestBox;
                requestBox.setWindowTitle("验证消息");
                requestBox.setText(QString("%1请求添加您为好友，是否同意？").arg(requesterName));
                requestBox.setStandardButtons(QMessageBox::Yes|QMessageBox::No);
                if(requestBox.exec() == QMessageBox::Yes)
                {
                    QString responseToServer = "##FRIEND_REQUEST_RETURN##";
                    //##FRIEND_REQUEST_RETURN##REQUESTER##USERNAME##STATUS
                    responseToServer.append(requesterName);
                    responseToServer.append("##");
                    responseToServer.append(userName);
                    responseToServer.append("##");
                    responseToServer.append("ACCEPTED");
                    qDebug()<<QString("已同意来自%1的好友请求，现在向服务器回应::%2").arg(requesterName).arg(responseToServer)<<endl;
                    tcpSocketToServer->write(responseToServer.toUtf8());
                    qDebug()<<"发送回应完成"<<endl;
                }
                else
                {
                    QString responseToServer = "##FRIEND_REQUEST_RETURN##";
                    responseToServer.append(requesterName);
                    responseToServer.append("##");
                    responseToServer.append(userName);
                    responseToServer.append("##");
                    responseToServer.append("REFUSED");
                    qDebug()<<QString("已拒绝来自%1的好友请求，现在向服务器回应::%2").arg(requesterName).arg(responseToServer)<<endl;
                    tcpSocketToServer->write(responseToServer.toUtf8());
                    qDebug()<<"发送回应完成"<<endl;
                }
            }
            else
            {
                qDebug()<<"收到了不正确的指令信息！"<<endl;
            }

        }
        else
            //在线消息
            //sender##MSG##recipient
        {
            QString friendName = response.section("##",0,0);
            QString msg = response.section("##",1,1);
            QString receiverName = response.section("##",2,2);
            ui->textBrowser->append("--------------"+QTime::currentTime().toString("hh:mm")+"--------------");
            ui->textBrowser->append(QString("<font color=orange>%1:</font><font color=black>%2</font>").arg(friendName).arg(msg));
        }
    });
}

void PowerChatClient::connectFailed()
{
    QMessageBox::information(this,tr("连接错误"),tr("连接服务器超时！"));
    timer->stop();
}

void PowerChatClient::on_btn_clear_clicked()//清除要发送的消息
{
    ui->lineEdit_msg->clear();
}

/*
 * 以下代码段为槽函数
*/
void PowerChatClient::on_btn_send_clicked()//发送消息
{
    if(ui->lineEdit_msg->text().trimmed()!="")
    {
        QString senderName = userName;
        QString recipientName = "";
        if(ui->comboBox_offline->currentIndex()!=0)
        {
            recipientName = ui->comboBox_offline->currentText();
        }
        else if(ui->comboBox_online->currentIndex()!=0)
        {
            QStringList temp = ui->comboBox_online->currentText().split("-");
            recipientName = temp.first().trimmed();
        }
        else
        {
            ui->textBrowser->append("System:请选择您要发送消息的好友");
            return;
        }
        QString msgToSend = senderName.append(QString("##%1##").arg(ui->lineEdit_msg->text())).append(recipientName);


        tcpSocketToServer->write(msgToSend.toUtf8());
        qDebug()<<"MESSAGE HAS SENT TO SERVER::"<<msgToSend<<endl;

        QString msg = QString("<font color=blue>%1:</font><font color=black>%2</font>")
                .arg(userName).arg(ui->lineEdit_msg->text()); //将自己发的消息显示上来

        ui->textBrowser->append("--------------" + QTime::currentTime().toString("hh:mm") + "---------------");
        ui->textBrowser->append(msg);
        ui->lineEdit_msg->clear();
    }
}


void PowerChatClient::on_comboBox_online_currentIndexChanged(int index)//从在线好友列表中找到要联系的好友
{
    if(index!=0)
    {
        if(ui->comboBox_online->currentText()==this->userName)
            ui->comboBox_online->setCurrentIndex(0);
        else
        {
            ui->btn_send->setEnabled(true);
            ui->comboBox_offline->setCurrentIndex(0);
            ui->comboBox_offline->setEnabled(false);
            for (int i = 0;i < userAmount;i++)
            {
                if(userList[i]->userName == ui->comboBox_online->currentText())
                {
                    ui->lineEdit_IP->setText(userList[i]->IP);
                    ui->lineEdit_port->setText(QString("%1").arg(userList[i]->port));
                }
            }
        }
    }
    else
        ui->comboBox_offline->setEnabled(true);
}

void PowerChatClient::on_comboBox_offline_currentIndexChanged(int index)//从离线好友列表中找到要联系的好友
{
    if(index == 0)
        ui->comboBox_online->setEnabled(true);
    else
    {
        ui->comboBox_online->setEnabled(false);
        ui->comboBox_online->setCurrentIndex(0);
        ui->btn_send->setEnabled(true);
    }
}

void PowerChatClient::on_btn_contact_disconnect_clicked()//不想和你聊了，重置UI
{
    ui->comboBox_online->setCurrentIndex(0);
    ui->comboBox_offline->setCurrentIndex(0);
    ui->textBrowser->clear();
    ui->lineEdit_msg->clear();
    ui->lineEdit_IP->setText("120.78.235.195");
    ui->lineEdit_port->setText("10086");
}

void PowerChatClient::on_comboBox_status_currentIndexChanged(int index)//用户状态变更
{
    if(ui->comboBox_status->currentIndex()==1)
    {
        if(!tcpSocketToServer->isOpen())
        {
            qDebug()<<"CONNECTING TO SERVER"<<endl;
            QString IP = "120.78.235.195";
            quint16 port = 10086;//测试服端口8800（仅手动收发调试指令）       正式服端口10086

            ui->lineEdit_IP->setText(IP);
            ui->lineEdit_port->setText(QString("%1").arg(port));

            tcpSocketToServer->connectToHost(QHostAddress(IP),port);
            tcpSocketToServer->write("##LOGIN_REQUEST");//##LOGIN_REQUEST

            qDebug()<<"Client:Request for Login"<<endl;
            timer->start(1000);
        }
    }

    else if(ui->comboBox_status->currentIndex() == 0)
    {
        for (int i=0; i<userAmount; i++)
        {
            delete userList[i];
            userList[i] = nullptr;
        }
        userAmount = 0;
        if(tcpSocketToServer->isOpen())
        {
            tcpSocketToServer->disconnectFromHost();
            //            tcpSocketToServer->close();
        }
    }

    else
    {
        tcpSocketToServer->write(QString("##STATUS_CHANGE_REQUEST##%1##%2").arg(userName).arg(index).toUtf8());
    }
}

void PowerChatClient::on_btn_close_clicked()
{
    for (int i=0; i<userAmount; i++)
    {
        delete userList[i];
        userList[i] = nullptr;
    }
    userAmount = 0;
    if(tcpSocketToServer->isOpen())
        tcpSocketToServer->disconnectFromHost();

    this->close();
}

void PowerChatClient::on_btn_min_clicked()
{
    this->setWindowState(Qt::WindowMinimized);
}

void PowerChatClient::on_btn_addFriends_clicked()
{
    ad->show();
    emit addWithSocket(userName,tcpSocketToServer);
}
