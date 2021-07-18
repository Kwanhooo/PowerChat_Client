#ifndef USER_H
#define USER_H

#include <QString>

class User
{
private:

public:
    User(QString userName,int status,QString IP,int port,bool isConnected = false);
    QString userName;
    int status;
    /*
     * 存储用户的状态
     */

    QString IP;
    int port;
    bool isConnected;

};

#endif // USER_H
