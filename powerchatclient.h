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
#include <QtGui>

#include <QMouseEvent>
#include <QPoint>

#include <QTimer>
#include <QTime>

#include <QVBoxLayout>

#include "user.h"
#include "adddialog.h"
#include "deletedialog.h"
#include "chatwidget.h"
#include "dailyattendance.h"

namespace Ui {
class PowerChatClient;
}


class QPoint;
class QPaintEvent;

class PowerChatClient : public QMainWindow
{
    Q_OBJECT

public:
    explicit PowerChatClient(QWidget *parent = nullptr);
    ~PowerChatClient();

private slots:
    void on_comboBox_online_currentIndexChanged(int index);
    void on_comboBox_offline_currentIndexChanged(int index);
    void on_btn_contact_disconnect_clicked();
    void on_comboBox_status_currentIndexChanged(int index);
    void on_btn_close_clicked();
    void on_btn_min_clicked();
    void on_btn_addFriends_clicked();
    void on_btn_contact_open_clicked();
    void on_btn_iknow_clicked();
    void on_btn_card_clicked();

    void on_btn_chatRoom_clicked();

    void on_btn_deleteFriends_clicked();

signals:
    //添加好友窗口所需信号
    void addWithSocket(QString,QTcpSocket*);
    void addResponse(QString);

    //删除好友窗口所需信号
    void deleteWithSocket(QString,QTcpSocket*);
    void deleteResponse(QString);

    //鼠标事件
    void mouseButtonMove(QPoint pos);
    void signalMainWindowMove();

    //聊天窗口
    void throwToChatWindow(QString);

    //打卡窗口
    void passAttendanceInfo(QString,QString,QTcpSocket*);
    void throwToAttendance(QString);
    void updateAttendanceAmount(QString);

protected:
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;

private:
    Ui::PowerChatClient *ui;

    //TCP
    QTcpSocket * tcpSocketToServer; //与服务器端的通信
    QTimer* timer;//检测与服务器的连接是否超时

    //一些必要参数以及初始化
    QString userName;
    bool hasLoadOffline;//是否已经请求加载离线消息
    bool isConnectedToServer;
    static const int MAX_USERS_AMOUNT = 20;
    User *userList[MAX_USERS_AMOUNT];
    QString attendaceAmount;
    QString avatar;
    QString Bio;
    int userAmount;//实际所有用户总数
    void initParameter();

    //一些连接相关函数
    void firstConnectWithServer();
    void initClientConfig();
    void setupTCP();
    void connectFailed();

    //鼠标事件所需变量
    bool m_Drag = false;
    QPoint m_DragPosition;

    //其他窗口
    AddDialog *ad;

    DeleteDialog *dd;

    DailyAttendance *daily;

    ChatWidget *groupChatWindow;
    QString groupOnlineAmount;
};

#endif // POWERCHATCLIENT_H
