#include "serial.h"
#include <QDebug>

Serial::Serial(QObject *parent) : QObject(parent)
{
    connect(&Serials, &QSerialPort::readyRead, this, &Serial::Serial_read);
}

int Serial::OpenSerial(const QString &str)
{
    Serials.setPortName(str);
    Serials.setBaudRate(115200);
    Serials.setDataBits(QSerialPort::Data8);
    Serials.setStopBits(QSerialPort::OneStop);
    Serials.setParity(QSerialPort::NoParity);
    Serials.setFlowControl(QSerialPort::NoFlowControl);
    //如果串口打开状态则关闭串口
    if(Serials.isOpen())
    {
       Serials.close();
    }
    //打开串口
    if(Serials.open(QIODevice::ReadWrite)==false)
    {
        return 1;
    }
    return  0;
}

QList<QString> Serial::scanSerialPorts(void) {
    QList<QString> portNames;
    QList<QSerialPortInfo> availablePorts = QSerialPortInfo::availablePorts();
    for(const QSerialPortInfo &portInfo : availablePorts) {
        portNames.append(portInfo.portName());
    }
    return portNames;
}

void Serial::CloseSerial(void)
{
    Serials.close();
}

void Serial:: SerialWrites(const unsigned char *pszData, unsigned int nCount)
{
    if (Serials.isOpen())
    {
        Serials.clear(QSerialPort::Output);
        Serials.write(reinterpret_cast<const char *>(pszData), nCount);
        if (!Serials.waitForBytesWritten(1000))  // 等待1秒
        {
            qDebug() << "Failed to write all bytes or timed out";
        }
    }
}

void Serial::Serial_read(void)
{
    //读取串口收到的数据
    QByteArray buffer = Serials.readAll();
    char *ReceiveBuff;
    int len;
    //QByteArray 转 char*
    ReceiveBuff = buffer.data();
    len = buffer.size();
    if(globalCallback)
    {
         globalCallback((const unsigned char*)ReceiveBuff,len);
    }
}

void Serial::registerCallback(CallbackSerialReceive callback)
{
    globalCallback = callback;
}

