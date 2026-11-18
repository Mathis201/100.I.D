#include "DeviceHandler.h"
#include <QDebug>
#include "../../Shared/Messaging/ble_setup.h"

static QBluetoothUuid toUuid(const std::string &s)
{
    return QBluetoothUuid(QString::fromStdString(s));
}

DeviceHandler::DeviceHandler(QObject *parent) : QObject(parent)
{
    m_discoveryAgent = new QBluetoothDeviceDiscoveryAgent(this);
    m_discoveryAgent->setLowEnergyDiscoveryTimeout(15000);

    connect(m_discoveryAgent, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered,
            this, &DeviceHandler::addDevice);
    connect(m_discoveryAgent, &QBluetoothDeviceDiscoveryAgent::finished,
            this, &DeviceHandler::scanFinished);
}

void DeviceHandler::startScan()
{
    qDebug() << "Searching for BLE device";
    m_discoveryAgent->start(QBluetoothDeviceDiscoveryAgent::LowEnergyMethod);
}

void DeviceHandler::addDevice(const QBluetoothDeviceInfo &device)
{
    qDebug() << "Discovered:" << device.name() << device.address().toString();

    if (device.name() == QString::fromStdString(ServerSettings::serverName)) {
        qDebug() << "Robus found" << device.name();
        m_discoveryAgent->stop();

        m_control = QLowEnergyController::createCentral(device, this);

        connect(m_control, &QLowEnergyController::connected, this, [this]() {
            qDebug() << "Connected to Robus. Searching for services";
            m_control->discoverServices();
        });

        connect(m_control, &QLowEnergyController::discoveryFinished,
                this, &DeviceHandler::serviceScanDone);

        m_control->connectToDevice();
    }
}

void DeviceHandler::scanFinished()
{
    qDebug() << "Scan done.";
}

void DeviceHandler::serviceScanDone()
{
    const std::string uuids[] = {
        GameInfoService::UUID,
        PlayerInfoService::UUID,
        RobotInfoService::UUID
    };

    m_pendingServices = 0;
    for (const auto &u : uuids) {
        QBluetoothUuid id = toUuid(u);
        QLowEnergyService *svc = m_control->createServiceObject(id, this);
        if (!svc) {
            qDebug() << "Service unavailable:" << id;
            continue;
        }
        m_services.insert(id, svc);
        ++m_pendingServices;

        connect(svc, &QLowEnergyService::stateChanged, this,
                [this](QLowEnergyService::ServiceState s) {
                    if (s == QLowEnergyService::RemoteServiceDiscovered && --m_pendingServices == 0)
                        emit ready();
                });
        connect(svc, &QLowEnergyService::characteristicRead, this,
                [this](const QLowEnergyCharacteristic &c, const QByteArray &v) {
                    emit valueReceived(c.uuid(), v);
                });
        connect(svc, &QLowEnergyService::characteristicChanged, this,
                [this](const QLowEnergyCharacteristic &c, const QByteArray &v) {
                    emit valueReceived(c.uuid(), v);
                });

        svc->discoverDetails();
    }
}

QLowEnergyCharacteristic DeviceHandler::findChar(const std::string &serviceUuid, const std::string &charUuid)
{
    QLowEnergyService *svc = m_services.value(toUuid(serviceUuid), nullptr);
    if (!svc || svc->state() != QLowEnergyService::RemoteServiceDiscovered)
        return QLowEnergyCharacteristic();
    return svc->characteristic(toUuid(charUuid));
}

void DeviceHandler::writeValue(const std::string &serviceUuid, const std::string &charUuid, const QByteArray &data)
{
    QLowEnergyCharacteristic c = findChar(serviceUuid, charUuid);
    if (!c.isValid()) { qDebug() << "Characteristic not ready"; return; }
    m_services[toUuid(serviceUuid)]->writeCharacteristic(c, data, QLowEnergyService::WriteWithResponse);
}

void DeviceHandler::readValue(const std::string &serviceUuid, const std::string &charUuid)
{
    QLowEnergyCharacteristic c = findChar(serviceUuid, charUuid);
    if (!c.isValid()) { qDebug() << "Characteristic not ready"; return; }
    m_services[toUuid(serviceUuid)]->readCharacteristic(c);   // result arrives via valueReceived
}

void DeviceHandler::subscribe(const std::string &serviceUuid, const std::string &charUuid)
{
    QLowEnergyCharacteristic c = findChar(serviceUuid, charUuid);
    if (!c.isValid()) return;
    QLowEnergyDescriptor cccd = c.descriptor(QBluetoothUuid::DescriptorType::ClientCharacteristicConfiguration);
    if (cccd.isValid())
        m_services[toUuid(serviceUuid)]->writeDescriptor(cccd, QByteArray::fromHex("0100"));
}




