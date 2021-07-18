#include "user.h"

User::User(QString userName, int status, QString IP, int port,bool isConnected)
{
    this->userName = userName;
    this->status = status;
    this->IP = IP;
    this->port = port;
    this->isConnected = isConnected;
}
