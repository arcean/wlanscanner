#ifndef CELL_H
#define CELL_H

#include <MWidget>
#include <QModelIndex>
#include <MLabel>
#include <MListItem>

class Cell : public MListItem
{
    Q_OBJECT
public:
    explicit Cell(MWidget *parent = 0);

    void updateCell(const QModelIndex& index, MWidget * cell) const;

    void setLine1(const QString& text);
    
signals:
    
public slots:

private:
    MLabel *line1;
    MLabel *line2;
    MLabel *line3;
    MLabel *date;
    
};

#endif // CELL_H
