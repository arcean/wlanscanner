#include <iostream>
#include <iomanip>

#include <wlancond.h>
#include <wlancond-dbus.h>

#include "dbushandler.h"



DbusHandler::DbusHandler() :
        iface(WLANCOND_SERVICE, WLANCOND_REQ_PATH, WLANCOND_REQ_INTERFACE, QDBusConnection::systemBus())
{
    if(!this->iface.isValid()){
        std::cerr << "Could not create D-Bus interface." << std::endl;
        throw std::exception();
    }

    QDBusConnection::systemBus().connect(
            WLANCOND_SERVICE,
            WLANCOND_SIG_PATH,
            WLANCOND_SIG_INTERFACE,
            WLANCOND_SCAN_RESULTS_SIG,
            this,
            SLOT(ScanResults(const QDBusMessage&))
        );
}



//Translate encryption type bit code to readable string describing
//the encryption type.
static std::string EncryptionBitsToString(unsigned bits){
    switch(bits & WLANCOND_ENCRYPT_METHOD_MASK){
        default:
        case WLANCOND_OPEN:
            return "NONE";
            break;
        case WLANCOND_WEP:
            return "WEP";
            break;
        case WLANCOND_WPA_PSK:
            return "WPA_PSK";
            break;
        case WLANCOND_WPA_EAP:
            return "WPA_EAP";
            break;
    }
}



//Slot to receive a result of scan for wireless networks request, from wlancond daemon.
void DbusHandler::ScanResults(const QDBusMessage& reply){
    QList<QVariant> l = reply.arguments();
    QList<ScanResult> networks;
    //ScanResult *result;

    for(int i = 1; i < l.size();){
        QVariant essid = l.at(i);
        ++i;
        QString essidStr = essid.toString().toUtf8().constData();
        //std::cout << "ESSID: " << essid.toString().toUtf8().constData() << std::endl;

        QVariant bssid = l.at(i);
        ++i;

            QByteArray a = bssid.toByteArray();
            QString addressStr = QString::number(int(a.constData()[0]))
                    + ":" + QString::number(int(a.constData()[1]))
                    + ":" + QString::number(int(a.constData()[2]))
                    + ":" + QString::number(int(a.constData()[3]))
                    + ":" + QString::number(int(a.constData()[4]))
                    + ":" + QString::number(int(a.constData()[5]));

            //std::string address;
        //    address = std::setw(2);

            std::cout << "Address: "
                    << std::setw(2) << std::setfill('0') << std::hex << int(a.constData()[0]) << ":"
                    << std::setw(2) << std::setfill('0') << std::hex << int(a.constData()[1]) << ":"
                    << std::setw(2) << std::setfill('0') << std::hex << int(a.constData()[2]) << ":"
                    << std::setw(2) << std::setfill('0') << std::hex << int(a.constData()[3]) << ":"
                    << std::setw(2) << std::setfill('0') << std::hex << int(a.constData()[4]) << ":"
                    << std::setw(2) << std::setfill('0') << std::hex << int(a.constData()[5])
                    << std::endl;


        QVariant rssi = l.at(i);
        ++i;
        //std::cout << "Signal level: " << std::dec << rssi.toInt() << std::endl;
        QString rssiStr = QString::number(rssi.toInt());

        QVariant channel = l.at(i);
        ++i;
        //std::cout << "Channel: " << std::dec << channel.toUInt() << std::endl;
        QString channelStr = QString::number(channel.toUInt());

        QVariant capbits = l.at(i);
        ++i;
        //std::cout << "Capbits: " << "0x" << std::hex << capbits.toUInt() << std::endl;
        QString capbitsStr = QString::number(capbits.toUInt());

        //std::cout << "Encryption: " << EncryptionBitsToString(capbits.toUInt()) << std::endl;
        QString encryptionStr = QString::fromStdString(EncryptionBitsToString(capbits.toUInt()));

        //std::cout << std::endl;

       // result = new ScanResult(essidStr, addressStr, rssiStr, channelStr, capbitsStr, encryptionStr);
        networks.append(ScanResult(essidStr, addressStr, rssiStr, channelStr, capbitsStr, encryptionStr));
    }

    emit this->scanComplete(networks);
}



//Request wlancond daemon to scan for available wireless networks.
//The result containing list of available wireless networks will be
//delivered to ScanResults() slot.
void DbusHandler::Scan(){
    std::cout << "Scanning..." << std::endl;
    this->iface.call(
            WLANCOND_SCAN_REQ,
            QVariant(int(WLANCOND_TX_POWER100)),
            QString("").toUtf8(),
            QVariant(unsigned(0))
        );
}



//Slot to receive a result of connect to wireless network request,
//sent to wlancond daemon.
void DbusHandler::ConnectResult(const QDBusMessage& reply){
    std::cout << "Connect result" << std::endl;
    QList<QVariant> l = reply.arguments();

    for(int i = 0; i < l.size(); ++i){
        std::cout << l[i].toString().toUtf8().constData() << std::endl;
    }
}



//Send a connect to wireless network request to wlancond daemon.
//The result will be delivered to ConnectResult() slot.
void DbusHandler::Connect(QString ssid, QString encryption, QString key){
    std::cout << "Connecting..." << std::endl;

    QList<QVariant> args;

    args << QVariant(int(WLANCOND_TX_POWER100)); // power level

    //The SSID should include the terminating 0.
    args << ssid.toUtf8().append(char(0)); // SSID

    int encryptionType = int(WLANCOND_OPEN);
    if(encryption == "open"){
        encryptionType = int(WLANCOND_OPEN);
    }else if(encryption == "wep"){
        encryptionType = int(WLANCOND_WEP);
    }else if(encryption == "wpapsk"){
        encryptionType = int(WLANCOND_WPA_PSK);
    }else if(encryption == "wpaeap"){
        encryptionType = int(WLANCOND_WPA_EAP);
    }

    args << QVariant(int(WLANCOND_INFRA)); //mode
    args << QVariant(encryptionType); //encryption
    args << key.toUtf8(); //key to be used with encryption (password)
    args << QByteArray(); //dummy
    args << QByteArray(); //dummy
    args << QByteArray(); //dummy
    args << QVariant(int(1)); //default key
    args << QVariant(unsigned(0)); //adhoc_channel

    this->iface.callWithCallback(
            WLANCOND_SETTINGS_AND_CONNECT_REQ,
            args,
            this,
            SLOT(ConnectResult(const QDBusMessage&))
        );
}



//Slot to receive a result of Disconnect request from wlancond daemon.
void DbusHandler::DisconnectResult(const QDBusMessage& reply){
    std::cout << "Disconnected" << std::endl;

    QList<QVariant> l = reply.arguments();

    for(int i = 0; i < l.size(); ++i){
        std::cout << l[i].toString().toUtf8().constData() << std::endl;
    }
}



//Request disconnecting from wireless network.
//The request is sent to wlancond over dbus.
//The result will be delivered to DisconnectResult() slot.
void DbusHandler::Disconnect(){
    std::cout << "Disconnecting..." << std::endl;

    QList<QVariant> args;

    this->iface.callWithCallback(
            WLANCOND_DISCONNECT_REQ,
            args,
            this,
            SLOT(DisconnectResult(const QDBusMessage&))
        );
}



//Slot to receive result of currently connected network status request.
void DbusHandler::StatusResult(const QDBusMessage& reply){
    QList<QVariant> l = reply.arguments();

    std::cout << "ESSID: " << l[0].toString().toUtf8().constData() << std::endl;

    {
        QByteArray a = l[1].toByteArray();
        std::cout << "Address: "
                << std::setw(2) << std::setfill('0') << std::hex << int(a.constData()[0]) << ":"
                << std::setw(2) << std::setfill('0') << std::hex << int(a.constData()[1]) << ":"
                << std::setw(2) << std::setfill('0') << std::hex << int(a.constData()[2]) << ":"
                << std::setw(2) << std::setfill('0') << std::hex << int(a.constData()[3]) << ":"
                << std::setw(2) << std::setfill('0') << std::hex << int(a.constData()[4]) << ":"
                << std::setw(2) << std::setfill('0') << std::hex << int(a.constData()[5])
                << std::endl;
    }

    std::cout << "Signal level: " << std::dec << l[2].toInt() << std::endl;

    std::cout << "Channel: " << std::dec << l[3].toUInt() << std::endl;

    std::cout << "Capbits: " << "0x" << std::hex << l[4].toUInt() << std::endl;

    std::cout << "Encryption: " << EncryptionBitsToString(l[5].toUInt()) << std::endl;

    std::cout << "Interface: " << l[6].toString().toUtf8().constData() << std::endl;
}



//Request status of currently connected network from wlancond daemon.
//The result will be delivered to StatusResult() slot.
void DbusHandler::Status(){
    QList<QVariant> args;

    this->iface.callWithCallback(
            WLANCOND_STATUS_REQ,
            args,
            this,
            SLOT(StatusResult(const QDBusMessage&))
        );
}



//Slot to receive a result of connection status request.
void DbusHandler::ConnectionStatusResult(const QDBusMessage& reply){
    QList<QVariant> l = reply.arguments();

    if(l.size() >= 1){
        if(l[0].toBool()){
            std::cout << "Connected" << std::endl;
        }else{
            std::cout << "Disconnected" << std::endl;
        }
    }else{
        std::cout << "Error" << std::endl;
    }
}



//Request connection status from wlancond daemon.
//The result will be delivered to ConnectionStatusResult() slot.
void DbusHandler::ConnectionStatus(){
    QList<QVariant> args;

    this->iface.callWithCallback(
            WLANCOND_CONNECTION_STATUS_REQ,
            args,
            this,
            SLOT(ConnectionStatusResult(const QDBusMessage&))
        );
}
