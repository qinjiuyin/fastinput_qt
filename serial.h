#ifndef SERIAL_H
#define SERIAL_H

#include <QObject>
#include <QSerialPortInfo>
#include <QSerialPort>

typedef void (*CallbackSerialReceive)(const unsigned char * /*pData*/ , unsigned int /*lenght*/);

class Serial : public QObject
{
    Q_OBJECT
public:
    explicit Serial(QObject *parent = 0);
    int OpenSerial(const QString &str);
    void CloseSerial(void);
    QList<QString> scanSerialPorts(void);
    void SerialWrites(const unsigned char *pszData, unsigned int nCount);
    void registerCallback(CallbackSerialReceive callback); //½ÓÊÕº¯Êý×¢²á
public slots:
    void Serial_read();
private:
    CallbackSerialReceive globalCallback = nullptr;
    QSerialPort Serials;
};

#endif // SERIAL_H
