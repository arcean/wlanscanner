#include "mlistitemcreator.h"

void MListItemCreator::updateCell(const QModelIndex& index, MWidget * cell) const
{
    MBasicListItem * contentItem = qobject_cast<MBasicListItem *>(cell);
    contentItem->setObjectName("NotesPopupListItem");
    contentItem->setItemStyle(MBasicListItem::TitleWithSubtitle);

    QVariant data = index.data(Qt::DisplayRole);
    QStringList rowData = data.value<QStringList>();
    QString title = rowData[0];

    title = title.remove(QRegExp("<[^>]*>"));
    title = title.remove("p, li { white-space: pre-wrap; }", Qt::CaseSensitive);

    contentItem->setTitle(title);
}
