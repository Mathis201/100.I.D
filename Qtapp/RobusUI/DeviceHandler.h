#ifndef DEVICEHANDLER_H
#define DEVICEHANDLER_H

#include <QObject>
#include <QBluetoothDeviceDiscoveryAgent>
#include <QLowEnergyController>
#include <QLowEnergyService>

class DeviceHandler : public QObject
{
    Q_OBJECT
public:
    ~DeviceHandler();
    explicit DeviceHandler(QObject *parent = nullptr);
    void startScan();

    void writeValue(const std::string &serviceUuid, const std::string &charUuid, const QByteArray &data);
    void readValue(const std::string &serviceUuid, const std::string &charUuid);
    void subscribe(const std::string &serviceUuid, const std::string &charUuid); // for notifications

signals:
    void ready();                                   // all services discovered
    void valueReceived(const QBluetoothUuid &charUuid, const QByteArray &value);

private slots:
    void addDevice(const QBluetoothDeviceInfo &device);
    void scanFinished();
    void serviceScanDone();

private:
    QLowEnergyCharacteristic findChar(const std::string &serviceUuid, const std::string &charUuid);

    QBluetoothDeviceDiscoveryAgent *m_discoveryAgent = nullptr;
    QLowEnergyController *m_control = nullptr;
    QHash<QBluetoothUuid, QLowEnergyService*> m_services;
    int m_pendingServices = 0;
};

#endif // DEVICEHANDLER_H
