#include "dailyattendance.h"
#include "ui_dailyattendance.h"

DailyAttendance::DailyAttendance(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DailyAttendance)
{
    ui->setupUi(this);
    this->setFixedSize(this->width(),this->height());//不让用户拉伸窗口大小
    this->setWindowTitle("PowerChat 打卡");
    this->setWindowFlag(Qt::FramelessWindowHint);
    ui->titleBarGroup->setAlignment(Qt::AlignRight);

    QDateTime currentDateTime = QDateTime::currentDateTime();
    QString currentMonth = currentDateTime.toString("MM");
    currentMonth = QString("%1月").arg(currentMonth.toInt());
    ui->label_month->setText(currentMonth);
    ui->label_day->setText(currentDateTime.toString("dd"));
    ui->label_week->setText(currentDateTime.toString("ddd"));
    ui->label_attendAmount->setText(QString("已有 %1 人打卡").arg(attendanceAmount));
    ui->label_attendAmount->setAlignment(Qt::AlignCenter);
}

DailyAttendance::~DailyAttendance()
{
    delete ui;
}

void DailyAttendance::on_btn_min_clicked()
{
    this->setWindowState(Qt::WindowMinimized);
}
void DailyAttendance::on_btn_close_clicked()
{
    this->hide();
}

void DailyAttendance::on_btn_attendance_clicked()
{
    //ATTENDANCE##USERNAME
    QString attendanceCommand = "##ATTENDANCE##";
    attendanceCommand.append(userName);
    tcpSocket->write(attendanceCommand.toUtf8());
}

void DailyAttendance::getAttendanceInfo(QString userName, QString attendanceAmount, QTcpSocket *tcpSocket)
{
    this->userName = userName;
    this->attendanceAmount = attendanceAmount;
    this->tcpSocket = tcpSocket;
    ui->label_attendAmount->setText(QString("已有 %1 人打卡").arg(attendanceAmount));
}

void DailyAttendance::getAttendanceResponse(QString response)
{
    //##ATTENDANCE_STATUS##STAUTS##ATTENDANCEAMOUNT
    if(response.section("##",2,2) == "SUCCESS")
    {
        this->attendanceAmount = response.section("##",3,3);
        ui->label_attendAmount->setText(QString("已有 %1 人打卡").arg(attendanceAmount));
        QMessageBox::information(this,"打卡成功","您已经打卡成功啦！");
    }
    else
    {
        QMessageBox::information(this,"打卡失败","您今天已经打卡咯");
    }
}
