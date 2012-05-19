#ifndef MLISTITEMCREATOR_H
#define MLISTITEMCREATOR_H

#include <MAbstractCellCreator>
#include <MBasicListItem>

class MListItemCreator : public MAbstractCellCreator<MBasicListItem>
{
public:
    void updateCell(const QModelIndex& index, MWidget * cell) const;

};

#endif // MLISTITEMCREATOR_H
