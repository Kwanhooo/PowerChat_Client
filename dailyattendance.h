#ifndef DAILYATTENDANCE_H
#define DAILYATTENDANCE_H

#include <QWidget>
#include <QTcpSocket>
#include <QTime>
#include <QMessageBox>

#include <QPoint>
#include <QMouseEvent>

namespace Ui {
class DailyAttendance;
}

class DailyAttendance : public QWidget
{
    Q_OBJECT

public:
    explicit DailyAttendance(QWidget *parent = nullptr);
    ~DailyAttendance();

private slots:
    void on_btn_min_clicked();
    void on_btn_close_clicked();
    void on_btn_attendance_clicked();

    void getAttendanceInfo(QString userName,QString attendanceAmount,QTcpSocket* tcpSocket);
    void getAttendanceResponse(QString response);

private:
    Ui::DailyAttendance *ui;

    QTcpSocket* tcpSocket;
    QString attendanceAmount;
    QString userName;



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

#endif // DAILYATTENDANCE_H
