#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "serial.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void SerialReceive(const unsigned char * p , unsigned int lenght);
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void myFunction();
private:
    bool SerialState;
    bool SerialCom;
    unsigned char SerialCnt;
    Serial mSerial;
    QTimer timer;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
