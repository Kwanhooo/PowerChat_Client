#ifndef DELETEDIALOG_H
#define DELETEDIALOG_H

#include <QWidget>

#include <QDialog>

#include <QTcpSocket>
#include <QtNetwork>

#include <QMessageBox>
#include <QMouseEvent>

namespace Ui {
class DeleteDialog;
}

class DeleteDialog : public QWidget
{
    Q_OBJECT

public:
    explicit DeleteDialog(QWidget *parent = nullptr);
    ~DeleteDialog();

private slots:
    void deleteGetTcpSocket(QString userName,QTcpSocket *tcpSocketParameter);
    void deleteGetResponse(QString response);

    void on_btn_cancel_clicked();

    void on_btn_min_clicked();

    void on_btn_close_clicked();

    void on_btn_search_clicked();

private:
    Ui::DeleteDialog *ui;
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

#endif // DELETEDIALOG_H
