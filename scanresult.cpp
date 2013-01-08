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

#include "scanresult.h"

ScanResult::ScanResult(QString essid, QString address, QString signalLevel, QString channel, QString capbits, QString encryption):
    essid(essid),
    address(address),
    signalLevel(signalLevel),
    channel(channel),
    capbits(capbits),
    encryption(encryption)
{
}

QString ScanResult::getAddress() const
{
    return address;
}

QString ScanResult::getCapbits() const
{
    return capbits;
}

QString ScanResult::getChannel() const
{
    return channel;
}

QString ScanResult::getEncryption() const
{
    return encryption;
}

QString ScanResult::getEssid() const
{
    return essid;
}

QString ScanResult::getSignalLevel() const
{
    return signalLevel;
}
