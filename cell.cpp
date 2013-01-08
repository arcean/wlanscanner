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

#include <QGraphicsLinearLayout>
#include <MImageWidget>
#include <MLabel>
#include <MImageWidget>
#include <QDebug>

#include "cell.h"


Cell::Cell (QGraphicsWidget *parent)
    : MBasicListItem(MBasicListItem::TitleWithSubtitle, parent)
{
    //setStyleName("CommonPanelInverted");
    signalStrength = NULL;
}

Cell::~Cell()
{
}

QGraphicsLayout *Cell::createLayout()
{
    qDebug() << "CREATE CELL";
    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Horizontal, this);
    QGraphicsLinearLayout *text = new QGraphicsLinearLayout(Qt::Vertical);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    //signalStrength = new MImageWidget();
    layout->addItem(signalStrength);
    layout->setAlignment(signalStrength, Qt::AlignLeft | Qt::AlignVCenter);

   // titleLabelWidget()->setStyleName("CommonTitleInverted");
    titleLabelWidget()->setAlignment(Qt::AlignTop);
    text->addItem(titleLabelWidget());
    text->setAlignment(titleLabelWidget(), Qt::AlignTop | Qt::AlignLeft);

   // subtitleLabelWidget()->setStyleName("CommonSubTitleInverted");
    text->addItem(subtitleLabelWidget());

    layout->addItem(text);

    QGraphicsWidget *spacer = new QGraphicsWidget;
    spacer->setContentsMargins(0, 0, 0, 0);
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    layout->addItem(spacer);

    image = new MImageWidget("icon-m-common-drilldown-arrow-inverse");
    layout->addItem(image);
    layout->setAlignment(image, Qt::AlignVCenter | Qt::AlignLeft);

    return layout;
}

void Cell::setSignalStrengthImage(QString image)
{
    qDebug() << "SET IMAGE";

    if (signalStrength == NULL) {
        qDebug() << "Creating new MImageWidget";
        signalStrength = new MImageWidget();
    }
    signalStrength->setImage(image);
}

void Cell::setDisabled(bool disable)
{
    setEnabled(!disable);
}
