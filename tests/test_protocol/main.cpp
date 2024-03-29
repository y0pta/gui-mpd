#include <QCoreApplication>
#include "cprotocoltransmitter.h"
#include "ssettings.h"
#include <QFile>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include "cserialport.h"

const QString pathToTestFile = "/home/liza/test";

void testRequestSettings()
{
    QFile file(pathToTestFile);
    file.open(QIODevice::WriteOnly);
    CProtocolTransmitter t(&file);
    t.setDevice(&file);
    t.getRequest(eNoSection);
    t.getRequest(eCommon);
    t.getRequest(eSerial);
    t.getRequest(eDebug);
    t.getRequest(eStat);

    t.serviceRequest(eClearStat);
    t.serviceRequest(eClearFlash);
    t.serviceRequest(eClearErrors);
    file.close();
}

void testSetSettings()
{
    QFile file(pathToTestFile);
    file.open(QIODevice::WriteOnly);

    CProtocolTransmitter t(&file);
    t.setDevice(&file);

    SCommonSection c;
    c.fields[COMMON_MODE] = "sms";
    c.fields[COMMON_DUMPSTATUS] = "on";
    t.setRequest(c);

    SSerialSection s;
    s.fields[SERIAL_DATABITS] = "8";
    s.fields[SERIAL_IFACE] = SERIAL_IFACE_RADIO;
    s.fields[SERIAL_PARITY] = "odd";
    s.fields[SERIAL_BAUDRATE] = "115200";
    t.setRequest(s);

    SDebugSection d;
    d.fields[DEBUG_CLEARSTAT] = "asdsads";
    t.setRequest(d);

    file.close();
}

void testReadData()
{
    QFile file_example(":/testReadResponse");
    file_example.open(QIODevice::ReadOnly);
    QFile file(pathToTestFile);
    file.open(QIODevice::WriteOnly);

    file.write(file_example.readAll());
    file.close();
    file_example.close();

    file.open(QIODevice::ReadOnly);
    CProtocolTransmitter t(&file);
    t.setDevice(&file);
    t.readyRead();
    file.close();
}

void testReadFailedConfirmed()
{
    QFile file_example(":/testReadFailedConfirmation");
    file_example.open(QIODevice::ReadOnly);
    QFile file(pathToTestFile);
    file.open(QIODevice::WriteOnly);

    file.write(file_example.readAll());
    file.close();
    file_example.close();

    file.open(QIODevice::ReadOnly);
    CProtocolTransmitter t(&file);
    t.setRequest(SSerialSection());
    t.setDevice(&file);
    t.readyRead();
    file.close();
}

void testReadConfirmed()
{
    QFile file_example(":/testReadConfirmed");
    file_example.open(QIODevice::ReadOnly);
    QFile file(pathToTestFile);
    file.open(QIODevice::WriteOnly);

    file.write(file_example.readAll());
    file.close();
    file_example.close();

    file.open(QIODevice::ReadOnly);
    CProtocolTransmitter t(&file);
    t.setRequest(SCommonSection());
    t.readyRead();
    file.close();
}

int main(int argc, char *argv[])
{
    // QCoreApplication a(argc, argv);
    QSerialPort serial;
    auto list = QSerialPortInfo::availablePorts();
    for (auto el : list) {
        qDebug() << el.portName();
        qDebug() << el.isBusy();
    }
    qDebug() << list.last().systemLocation();
    serial.setPort(list.last());
    serial.open(QSerialPort::ReadWrite);

    qDebug() << "opened";
    QString str = "get(common)\n";
    qDebug() << str;
    serial.write(QByteArray(str.toStdString().c_str()));
    /// если убрать эту строчку, то не будет работать
    serial.waitForBytesWritten(5000);
    while (serial.bytesAvailable()) {
        auto size = serial.bytesAvailable();
        auto s = serial.readAll();
        qDebug() << "size: " << size << "bytes ." << s.toStdString().c_str();
        /// если убрать эту строчку, то не будет работать
        serial.waitForBytesWritten(5000);
    }
    serial.close();
    // return a.exec();
    // testRequestSettings();
    // testSetSettings();
    // testReadConfirmed();
    // testReadFailedConfirmed();
    // testReadData();
}
