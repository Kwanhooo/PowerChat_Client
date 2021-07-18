#include "powerchatclient.h"
#include "logindialog.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    PowerChatClient w;//先不显示这个窗口，待登陆成功后显示

    return a.exec();
}
