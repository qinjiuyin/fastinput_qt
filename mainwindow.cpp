#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QDebug>

#pragma execution_character_set("utf-8")
static MainWindow* instace;

void mCallbackSerialReceive(const unsigned char *pData , unsigned int lenght)
{
    instace->SerialReceive(pData,lenght);
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    instace = this;
    ui->setupUi(this);
    ui->label_5->setNum(0);
    ui->label_6->setNum(0);
    ui->label_7->setNum(0);
    ui->label_8->setNum(0);
    SerialState = false;
    connect(&timer, &QTimer::timeout, this, &MainWindow::myFunction); //链接定时器
    mSerial.registerCallback(mCallbackSerialReceive);//注册回调 这里是因为后面想把串口编译成dll
    on_pushButton_clicked();//扫描一次串口
    SerialCnt = 0;
}

MainWindow::~MainWindow()
{
    delete ui;
}

//扫描串口
void MainWindow::on_pushButton_clicked()
{
    QList<QString> ports = mSerial.scanSerialPorts();
    for(const QString &port : ports) {
        ui->comboBox->addItem(port);
    }
}
// 打开串口
void MainWindow::on_pushButton_2_clicked()
{
    if(SerialState==false)
    {
        if(mSerial.OpenSerial(ui->comboBox->currentText()))
        {
            QMessageBox::warning(NULL , "Tips", "Serial Open failed");
        }
        else
        {
          SerialState = true;
          ui->pushButton_2->setText(QString::fromUtf8("关闭串口"));
          timer.start(10);
        }
    }
    else
    {
        mSerial.CloseSerial();
        SerialState = false;
        ui->pushButton_2->setText(QString::fromUtf8("打开串口"));
        timer.stop();
    }
}

void MainWindow::myFunction()
{
    if(SerialCom==false)
    {
        const unsigned char Send[1] = {0x05};
        mSerial.SerialWrites(Send,1);
        SerialCom = true;
    }
    else
    {
        SerialCnt++;
        if(SerialCnt>=5)
        {
            SerialCnt = 0;
            SerialCom = false;
        }
    }
}

void MainWindow::SerialReceive(const unsigned char * p , unsigned int lenght)
{
    unsigned int value[4];
    if(p[0]==0x06 && lenght==18)
    {
        value[0] = p[4]<<24|p[3]<<16|p[2]<<8|p[1];
        value[1] = p[8]<<24|p[7]<<16|p[6]<<8|p[5];
        value[2] = p[12]<<24|p[11]<<16|p[10]<<8|p[9];
        value[3] = p[16]<<24|p[15]<<16|p[14]<<8|p[13];
        int check = value [0] + value [1] + value [2] + value [3];
        if((check&0xf)==p[17])
        {
            ui->label_5->setText(QString::number(value[0]));
            ui->label_6->setText(QString::number(value[1]));
            ui->label_7->setText(QString::number(value[2]));
            ui->label_8->setText(QString::number(value[3]));
        }
    }
    SerialCom = false;
}




