#ifndef CSERIALPORT_H
#define CSERIALPORT_H

#include <QObject>
#include <QSerialPort>
#include <QTimer>
#include <QObject>
#include <ssettings.h>

class CSerialPort : public QObject
{
    Q_OBJECT
public:
    CSerialPort(QObject *parent = nullptr);
    ~CSerialPort() {}

public:
    static QList<QString> getAvaliable();

public slots:
    QByteArray readyRead();

signals:
    void s_readyRead();
    void s_error(QString);
    void s_disconnected();
    void s_connected();
    void s_avaliablePortsChanged(QList<QString>);

public:
    // opens port with default settings (115200 8n1)
    bool openDefault(QString name);
    bool close();
    void sendData(const SSettings &data);
    void sendData(const QByteArray &data);
    QList<SSettings> readAllSettings();
    QByteArray readAllRaw();

private:
    void checkAvaliablePorts();

private:
    QSerialPort m_port;
    QTimer m_timeToCheckPorts;
    QList<QString> m_avaliablePorts;
};

#endif // CSERIALPORT_H
