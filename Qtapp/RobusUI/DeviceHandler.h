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
    explicit DeviceHandler(QObject *parent = nullptr);
    void startScan();
    void sendButtonClicked(bool clicked);

private slots:
    void addDevice(const QBluetoothDeviceInfo &device);
    void scanFinished();
    void serviceDiscovered(const QBluetoothUuid &gatt);
    void serviceScanDone();

private:
    QBluetoothDeviceDiscoveryAgent *m_discoveryAgent = nullptr;
    QLowEnergyController *m_control = nullptr;
    QLowEnergyService *m_service = nullptr;
    QLowEnergyCharacteristic m_characteristic;

    const QBluetoothUuid SERVICE_UUID = QBluetoothUuid(QStringLiteral("37d5098d-a1bd-471e-8794-aae7bf8ed532"));
    const QBluetoothUuid CHAR_UUID    = QBluetoothUuid(QStringLiteral("4ca76883-83e7-4c4e-a3da-b433e5e8fd2a"));
};

#endif // DEVICEHANDLER_H
