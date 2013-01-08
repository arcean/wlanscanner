#pragma once

#include <QtDBus/QtDBus>

#include "scanresult.h"


class DbusHandler : public QObject{
    Q_OBJECT


    QDBusInterface iface;

public:
    DbusHandler();

    void Scan();

    void Connect(QString ssid, QString encryption = QString("open"), QString key = QString());

    void Disconnect();

    void Status();

    void ConnectionStatus();

signals:
    void scanComplete(const QList<ScanResult> &networks);

private slots:
    void ScanResults(const QDBusMessage& reply);

    void ConnectResult(const QDBusMessage& reply);

    void DisconnectResult(const QDBusMessage& reply);

    void StatusResult(const QDBusMessage& reply);

    void ConnectionStatusResult(const QDBusMessage& reply);
};
