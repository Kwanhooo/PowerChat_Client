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

#include <QMouseEvent>

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
    void on_comboBox_online_currentIndexChanged(int index);
    void on_comboBox_offline_currentIndexChanged(int index);
    void on_btn_contact_disconnect_clicked();
    void on_comboBox_status_currentIndexChanged(int index);

    void on_btn_close_clicked();

    void on_btn_min_clicked();

private:
    Ui::PowerChatClient *ui;

    QTcpSocket * tcpSocketToServer; //与服务器端的通信

    QTimer* timer;//检测与服务器的连接是否超时

    //一些必要参数以及初始化
    QString userName;
    bool hasLoadOffline;//是否已经请求加载离线消息
    static const int MAX_USERS_AMOUNT = 20;
    User *userList[MAX_USERS_AMOUNT];

    int userAmount;//实际所有用户总数
    void initParameter();
    void firstConnectWithServer();
    void initClientConfig();
    void setupTCP();
    void connectFailed();


    QPoint m_lastPos;
    bool isPressedWidget;

    void mousePressEvent(QMouseEvent *event)
    {
        m_lastPos = event->globalPos();
        isPressedWidget = true; // 当前鼠标按下的即是QWidget而非界面上布局的其它控件
    }

    void mouseMoveEvent(QMouseEvent *event)
    {
        if (isPressedWidget) {
            this->move(this->x() + (event->globalX() - m_lastPos.x()),
                       this->y() + (event->globalY() - m_lastPos.y()));
            m_lastPos = event->globalPos();
        }
    }

    void mouseReleaseEvent(QMouseEvent *event)
    {
        // 其实这里的mouseReleaseEvent函数可以不用重写
        m_lastPos = event->globalPos();
        isPressedWidget = false; // 鼠标松开时，置为false
    }
};

#endif // POWERCHATCLIENT_H
