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
#ifndef CELL_H
#define CELL_H

#include <MBasicListItem>

class MImageWidget;

class Cell: public MBasicListItem
{
    Q_OBJECT
public:
    Cell(QGraphicsWidget* parent = 0);
    ~Cell ();

    void setSignalStrengthImage(QString image);
    void setDisabled(bool disable);


protected:
    virtual QGraphicsLayout *createLayout();

private:
    MImageWidget *image;
    MImageWidget *signalStrength;

};

#endif // CELL_H
