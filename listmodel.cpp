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

#include <QDir>
#include <QFileInfoList>
#include <QDebug>

#include "listmodel.h"

ListModel::ListModel(QObject *parent)
   : MAbstractItemModel(parent)
{

}

void ListModel::reload(const QList<ScanResult> &networks)
{
    qDebug() << "reload";
    this->newNetworks = networks;
    qDebug() << "newNetworks updated";

   /* if(networks.count() > 0)
        qDebug() << networks.at(0).getEssid() << " " << networks.at(0).getEncryption();

    for (int i = 0; i < networks.count(); i++) {
        qDebug() << "dataChanged for" << i;
        emit dataChanged(createIndex(i, 0), createIndex(i, 0));
    }*/
}

int ListModel::rowCount(const QModelIndex &parent) const
{
   Q_UNUSED(parent);
   return networks.count();
}
/*
QVariant ListModel::data(const QModelIndex &index, int role) const
{
  if (role == Qt::DisplayRole) {
      QStringList rowData;
      rowData << filesData.at(index.row());

      return QVariant(rowData);
  }

  return QVariant();
}*/

int ListModel::groupCount() const
{
    return 0;
}

int ListModel::rowCountInGroup(int group) const
{
    return networks.count();
}

QString ListModel::groupTitle(int group) const
{
    return "";
}

QVariant ListModel::itemData(int row, int group, int role) const
{
    int flatRow = row;
    Q_ASSERT(flatRow >= 0);
    Q_ASSERT(flatRow < filesData.size());

    if (role == Qt::DisplayRole) {
        QStringList rowData;
        rowData << networks.at(row).getEssid();
        rowData << networks.at(row).getEncryption();
        rowData << networks.at(row).getSignalLevel();
        qDebug() << "ROWDATA" << rowData;
        return QVariant(rowData);
    }

    return QVariant();

}

bool ListModel::insertRows(int row, int count, const QModelIndex &parent)
{
    if (count <= 0)
        return true; // Successfully added 0 rows.

    QList<ScanResult> data;

    for (int i = row; i < row + count; i++) {
        data.insert(i, newNetworks.at(i));
    }  

    QModelIndex realParent = parent;
    beginInsertRows(realParent, row, row + count - 1, count == 1);
    networks = data;
    endInsertRows();

    return true;
}

bool ListModel::removeRows(int row, int count, const QModelIndex &parent)
{
    if (count <= 0)
        return true; //Successfully removed 0 rows.

    Q_ASSERT(row >= 0);
    Q_ASSERT(row < networks.size());

    beginRemoveRows(parent, row, row + count - 1, count == 1);
    //qDeleteAll(filesData.begin() + flatRow, filesData.begin() + flatRow + count - 1);
    networks.removeAt(row);
    endRemoveRows();

    return true;
}
