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

#include <MImageWidget>

#include "mlistitemcreator.h"

void MListItemCreator::updateCell(const QModelIndex& index, MWidget * cell) const
{
    Cell * contentItem = qobject_cast<Cell *>(cell);
  //  contentItem->setObjectName("CommonBasicListItem");
  //  contentItem->setItemStyle(MBasicListItem::TitleWithSubtitle);

    QVariant data = index.data(Qt::DisplayRole);
    QStringList rowData = data.value<QStringList>();
    QString title = rowData[0];
    int value = QString(rowData[2]).toInt();
    QString signalStrength;

    /*
     * WL1721 RSSI:
     * -89dBm = 0%
     * -49dBm = 100%
     */
    if (value < -85)
        signalStrength = "wlanscanner-wlan1";
    else if (value >= -85 && value < -75)
        signalStrength = "wlanscanner-wlan2";
    else if (value >= -75 && value < -65)
        signalStrength = "wlanscanner-wlan3";
    else if (value >= -65)
        signalStrength = "wlanscanner-wlan4";

    contentItem->setTitle(title);
    contentItem->setSubtitle(rowData[1]);
    contentItem->setSignalStrengthImage(signalStrength);
}
