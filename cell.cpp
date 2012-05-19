#include <MLayout>
#include <MLinearLayoutPolicy>
#include "cell.h"

Cell::Cell(MWidget *parent) :
    MListItem(parent)
{
    MLayout *layout = new MLayout;
    this->setLayout(layout);
    MLinearLayoutPolicy *portraitPolicy = new MLinearLayoutPolicy(layout, Qt::Vertical);
    layout->setPortraitPolicy(portraitPolicy);
    portraitPolicy->setContentsMargins(0, 0, 0, 0);
    portraitPolicy->setSpacing(0);
    portraitPolicy->setNotifyWidgetsOfLayoutPositionEnabled(true);

    line1 = new MLabel();
    line2 = new MLabel();
    line3 = new MLabel();
    date = new MLabel();

    line1->setWordWrap(false);
    line2->setWordWrap(false);
    line3->setWordWrap(false);
    date->setWordWrap(false);

    line1->setTextElide(true);
    line2->setTextElide(true);
    line3->setTextElide(true);
    date->setTextElide(true);

    portraitPolicy->addItem(line1);
    portraitPolicy->addItem(line2);
    portraitPolicy->addItem(line3);
    portraitPolicy->addItem(date);

    this->setStyleName("NotesPopupListItem");
}

void Cell::setLine1(const QString& text)
{
    line1->setText(text);
}

void Cell::updateCell(const QModelIndex& index, MWidget * cell) const
{
    Cell *contentItem = qobject_cast<Cell *>(cell);
    QVariant data = index.data(Qt::DisplayRole);
    QStringList rowData = data.value<QStringList>();
    contentItem->setLine1(rowData[0]);
    //contentItem->setSubtitle("rowData[1]");
}
