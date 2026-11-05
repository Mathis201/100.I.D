#include "devicehandler.h"
#include <QDebug>

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

    if (device.name().contains("ESP32")) {
        qDebug() << "Robus found" << device.name();
        m_discoveryAgent->stop();

        // Création du contrôleur BLE
        m_control = QLowEnergyController::createCentral(device, this);

        connect(m_control, &QLowEnergyController::connected, this, [this]() {
            qDebug() << "Connected to Robus. Searching for services ";
            m_control->discoverServices();
        });

        connect(m_control, &QLowEnergyController::serviceDiscovered,
                this, &DeviceHandler::serviceDiscovered);
        connect(m_control, &QLowEnergyController::discoveryFinished,
                this, &DeviceHandler::serviceScanDone);

        m_control->connectToDevice();
    }
}

void DeviceHandler::scanFinished()
{
    qDebug() << "Scan done.";
}

void DeviceHandler::serviceDiscovered(const QBluetoothUuid &gatt)
{
    if (gatt == SERVICE_UUID) {
        qDebug() << "Service found.";
    }
}

void DeviceHandler::serviceScanDone()
{
    m_service = m_control->createServiceObject(SERVICE_UUID, this);
    if (!m_service) {
        qDebug() << "Service unavailable";
        return;
    }

    connect(m_service, &QLowEnergyService::stateChanged, this, [this](QLowEnergyService::ServiceState state) {
        if (state == QLowEnergyService::RemoteServiceDiscovered) {
            m_characteristic = m_service->characteristic(CHAR_UUID);
            qDebug() << "System ready";
        }
    });

    m_service->discoverDetails();
}

void DeviceHandler::sendButtonClicked(bool clicked)
{
    if (!m_service || !m_characteristic.isValid()) {
        qDebug() << "Error, system not ready";
        return;
    }

    QByteArray data;
    data.append(clicked ? static_cast<char>(0x01) : static_cast<char>(0x00));

    // Écriture de la valeur sur l'ESP32
    m_service->writeCharacteristic(m_characteristic, data, QLowEnergyService::WriteWithResponse);
}
