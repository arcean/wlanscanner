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

#ifndef LISTMODEL_H
#define LISTMODEL_H

#include <QAbstractListModel>
#include <QList>
#include <MAbstractItemModel>
#include <QDateTime>

#include "scanresult.h"
#include "dbushandler.h"

class ListModel : public MAbstractItemModel
{
    Q_OBJECT

 public:
    ListModel(QObject *parent = 0);

    int rowCount(const QModelIndex &parent) const;
   // QVariant data(const QModelIndex &index, int role) const;
    bool removeRows(int row, int count, const QModelIndex &parent);
    bool insertRows(int row, int count, const QModelIndex &parent);
    QVariant itemData(int row, int group, int role) const;
    int rowCountInGroup(int group) const;
    QString groupTitle(int group) const;
    int groupCount() const;

 public slots:
    void reload(const QList<ScanResult> &networks);

 private:

    QList<ScanResult> networks;
    QList<ScanResult> newNetworks;
    DbusHandler *dbusHandler;

};

#endif // LISTMODEL_H
