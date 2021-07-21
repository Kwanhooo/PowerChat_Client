#include "user.h"

User::User(QString userName, int status, QString IP, int port,QString avatar,QString Bio)
{
    this->userName = userName;
    this->status = status;
    this->IP = IP;
    this->port = port;
    this->cw = nullptr;
    this->avatar = avatar;
    this->Bio = Bio;
}
