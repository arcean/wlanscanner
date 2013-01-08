/***************************************************************************
**
** Copyright (C) 2012 Tomasz Pieniążek
** All rights reserved.
** Contact: Tomasz Pieniążek <t.pieniazek@gazeta.pl>
**
** This program is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation
** and appearing in the file LICENSE.LGPL included in the packaging
** of this file.
**
****************************************************************************/

#ifndef SCANRESULT_H
#define SCANRESULT_H

#include <QString>

class ScanResult {

public:
    ScanResult(QString essid, QString address, QString signalLevel, QString channel, QString capbits, QString encryption);

    QString getEssid() const;
    QString getAddress() const;
    QString getSignalLevel() const;
    QString getChannel() const;
    QString getCapbits() const;
    QString getEncryption() const;

private:
    QString essid;
    QString address;
    QString signalLevel;
    QString channel;
    QString capbits;
    QString encryption;
};

#endif // SCANRESULT_H
