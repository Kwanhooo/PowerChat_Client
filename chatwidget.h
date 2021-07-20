#ifndef CHATWIDGET_H
#define CHATWIDGET_H

#include <QWidget>

#include <QMouseEvent>
#include <QPoint>

#include <QTcpSocket>

#include "adddialog.h"

namespace Ui {
class ChatWidget;
}

class ChatWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ChatWidget(QWidget *parent = nullptr);
    ~ChatWidget();
    void setupThisWindow(QString userNamePara, QString friendNamePara, QTcpSocket *tcpSocketPara,AddDialog* ad);

protected:
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;

private:
    Ui::ChatWidget *ui;
    QString userName;
    QString friendName;
    QTcpSocket* tcpSocket;
    QString response;
    AddDialog* ad;

    //鼠标事件所需变量
    bool m_Drag = false;
    QPoint m_DragPosition;

signals:
    //鼠标事件
    void mouseButtonMove(QPoint pos);
    void signalMainWindowMove();

private slots:
    void getChatMsg(QString responsePara);
    void on_btn_min_clicked();
    void on_btn_close_clicked();
    void on_btn_close_2_clicked();
    void on_btn_clear_clicked();
    void on_btn_addFriends_clicked();
    void on_btn_send_clicked();
};

#endif // CHATWIDGET_H
