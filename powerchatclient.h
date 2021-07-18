#ifndef POWERCHATCLIENT_H
#define POWERCHATCLIENT_H

#include <QMainWindow>
#include <QMessageBox>
#include <QFile>

#include <QTcpServer>
#include <QTcpSocket>
#include <QByteArray>
#include <QTextStream>
#include <QtNetwork>


#include <QTimer>
#include <QTime>

#include <QVBoxLayout>

#include "user.h"

namespace Ui {
class PowerChatClient;
}

class PowerChatClient : public QMainWindow
{
    Q_OBJECT

public:
    explicit PowerChatClient(QWidget *parent = nullptr);
    ~PowerChatClient();

private slots:
    void on_btn_clear_clicked();
    void on_btn_send_clicked();
    void on_comboBox_status_currentIndexChanged(const QString &arg1);
    void on_comboBox_online_currentIndexChanged(int index);
    void on_comboBox_offline_currentIndexChanged(int index);
    void on_btn_contact_disconnect_clicked();
private:
    Ui::PowerChatClient *ui;

    QTcpSocket * tcpSocketToServer; //与服务器端的通信

    QTimer* timer;//检测与服务器的连接是否超时

    //一些必要参数以及初始化
    QString userName;
    bool hasLoadOffline;//是否已经请求加载离线消息
    bool isUpdated;//是否已经
    static const int MAX_USERS_AMOUNT = 20;
    User *userList[MAX_USERS_AMOUNT];
    int userAmount;//用户总数
    void initParameter();



    void firstConnectWithServer();
    void initClientConfig();
    void setupTCP();
    void connectFailed();

};

#endif // POWERCHATCLIENT_H
