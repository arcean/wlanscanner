#include <QDir>
#include <QFileInfoList>
#include <QDebug>
#include "filemodel.h"

FileModel::FileModel(QObject *parent)
   : MAbstractItemModel(parent)
{
    numFiles = countFiles();

    for (int i = 0; i < numFiles; i++) {
        filesData.append(getTextFromFile(i));
    }
}

void FileModel::reload()
{
    numFiles = countFiles();
    qDebug() << "numFiles" << numFiles;
    filesData.clear();

    for (int i = 0; i < numFiles; i++) {
        filesData.append(getTextFromFile(i));
    }

    for (int i = 0; i < numFiles; i++)
        emit dataChanged(createIndex(i, 0), createIndex(i, 0));
}

int FileModel::countFiles()
{
    QDir files("/home/user/MyDocs/texteditor/");
    QFileInfoList fileList = files.entryInfoList(QDir::Files, QDir::Time);

    return fileList.length();
}

QString FileModel::getTextFromFile(int position)
{
    QDir files("/home/user/MyDocs/texteditor/");
    QFileInfoList fileList = files.entryInfoList(QDir::Files, QDir::Time);
    int fileListLength = fileList.length();

    if (position >= fileListLength || position < 0) {
        qDebug() << "[E]: No such file:" << position;
        return "";
    }

    QFile file (fileList.at(position).filePath());
    qDebug() << "FN:" << fileList.at(position).filePath();

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return "";

    QTextStream in(&file);
    QString line = in.readLine();

    while (!in.atEnd()) {
        line += in.readLine();
        //line += "\n";

        if (line.length() > 2100) {
            qDebug() << "break";
            break;
        }
    }

    return line;
}

int FileModel::rowCount(const QModelIndex &parent) const
{
   Q_UNUSED(parent);
   return filesData.size();
}
/*
QVariant FileModel::data(const QModelIndex &index, int role) const
{
  if (role == Qt::DisplayRole) {
      QStringList rowData;
      rowData << filesData.at(index.row());

      return QVariant(rowData);
  }

  return QVariant();
}*/

int FileModel::groupCount() const
{
    return 0;
}

int FileModel::rowCountInGroup(int group) const
{
    return filesData.count();
}

QString FileModel::groupTitle(int group) const
{
    return "";
}

QVariant FileModel::itemData(int row, int group, int role) const
{
    int flatRow = row;
    Q_ASSERT(flatRow >= 0);
    Q_ASSERT(flatRow < filesData.size());

    if (role == Qt::DisplayRole) {
        QStringList rowData;
        qDebug() << "index:" << row;
        rowData << filesData.at(row);
        return QVariant(rowData);
    }

    return QVariant();

}

bool FileModel::insertRows(int row, int count, const QModelIndex &parent)
{
    if (count <= 0)
        return true; // Successfully added 0 rows.

    QList<QString> data = filesData;
    QString entry;

    for (int i = row; i < row+count; i++) {
        entry = getTextFromFile(i);

        if (!entry.isEmpty())
            data.insert(i, entry);
    }

    QStringList entriesList;

    foreach (QString entry, data) {
        //Q_ASSERT(entry);
        entriesList.prepend(entry);
    }

    QModelIndex realParent = parent;
    beginInsertRows(realParent, row, row + count - 1, count == 1);
    filesData = data;
    endInsertRows();

    return true;

}

bool FileModel::removeRows(int row, int count, const QModelIndex &parent)
{
    if (count <= 0)
        return true; //Successfully removed 0 rows.

    int flatRow = row;

    Q_ASSERT(flatRow >= 0);
    Q_ASSERT(flatRow < filesData.size());

    beginRemoveRows(parent, row, row + count - 1, count == 1);
    //qDeleteAll(filesData.begin() + flatRow, filesData.begin() + flatRow + count - 1);
    filesData.removeAt(flatRow);
    endRemoveRows();

    return true;
}

QString FileModel::getFilePath(int index)
{
    QDir files("/home/user/MyDocs/texteditor/");
    QFileInfoList fileList = files.entryInfoList(QDir::Files, QDir::Time);
    int fileListLength = fileList.length();

    if (index >= fileListLength || index < 0) {
        qDebug() << "[E]: No such file:" << index;
        return "";
    }

    return fileList.at(index).filePath();
}
