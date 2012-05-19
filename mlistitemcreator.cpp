#include "mlistitemcreator.h"

void MListItemCreator::updateCell(const QModelIndex& index, MWidget * cell) const
{
    MBasicListItem * contentItem = qobject_cast<MBasicListItem *>(cell);
    contentItem->setObjectName("CommonBasicListItem");
    contentItem->setItemStyle(MBasicListItem::TitleWithSubtitle);

    QVariant data = index.data(Qt::DisplayRole);
    QStringList rowData = data.value<QStringList>();
    QString title = rowData[0];

    title = title.replace(QRegExp("<[^>]*/p>"), " ");
    title = title.remove(QRegExp("<[^>]*>"));
    title = title.remove("p, li { white-space: pre-wrap; }", Qt::CaseSensitive);

    contentItem->setTitle(title);
    contentItem->setSubtitle(rowData[1]);
}
