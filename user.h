#ifndef USER_H
#define USER_H

#include <QString>
#include "chatwidget.h"

class User
{
private:

public:
    User(QString userName,int status,QString IP,int port);
    QString userName;
    int status;
    /*
         * status指示用户的状态
         *
         * 0--离线
         * 1--在线
         * 2--Q我吧
         * 3--离开
         * 4--忙碌
         * 5--请勿打扰
         * 6--隐身
     */

    QString IP;
    int port;

    ChatWidget *cw;
};

#endif // USER_H
