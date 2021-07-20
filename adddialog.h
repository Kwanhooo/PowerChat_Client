#ifndef ADDDIALOG_H
#define ADDDIALOG_H

#include <QDialog>

#include <QTcpSocket>
#include <QtNetwork>

#include <QMessageBox>
#include <QMouseEvent>

namespace Ui {
class AddDialog;
}

class AddDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddDialog(QDialog *parent = nullptr);
    ~AddDialog();

private slots:
    void getTcpSocket(QString userName,QTcpSocket *tcpSocketParameter);

    void getResponse(QString response);

    void on_btn_cancel_clicked();

    void on_btn_search_clicked();

    void on_btn_min_clicked();

    void on_btn_close_clicked();

private:
    Ui::AddDialog *ui;

    QString userName;
    QTcpSocket *tcpSocket;

    //由于去除了标题栏（因为有点丑），所以要重写鼠标事件来实现窗口的拖动
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
        m_lastPos = event->globalPos();
        isPressedWidget = false;
    }
};

#endif // ADDDIALOG_H
