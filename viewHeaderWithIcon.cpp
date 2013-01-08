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

#include <MButton>
#include <MLayout>
#include <MLinearLayoutPolicy>

#include "viewHeaderWithIcon.h"

ViewHeaderWithIcon::ViewHeaderWithIcon(QGraphicsItem *parent) :
    MWidgetController(parent),
    linearLayout(0),
    titleWidget(0)
{
    setStyleName("CommonHeaderPanel");
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    MLayout *layout = new MLayout(this);
    MLinearLayoutPolicy *layoutPolicy = new MLinearLayoutPolicy(layout, Qt::Horizontal);
    titleWidget = new MLabel();
    titleWidget->setTextElide(true);
    titleWidget->setStyleName("CommonHeaderInverted");
    layoutPolicy->addItem(titleWidget);

    MButton *refreshButton = new MButton(this);
    refreshButton->setIconID ("icon-m-common-refresh");
    refreshButton->setViewType (MButton::iconType);
    refreshButton->setStyleName("CommonRightIcon");
    layoutPolicy->addItem(refreshButton);
    layoutPolicy->setAlignment(refreshButton, Qt::AlignLeft | Qt::AlignVCenter);
    connect(refreshButton, SIGNAL(clicked()), this, SLOT(onButtonClicked()));
}

void ViewHeaderWithIcon::onButtonClicked()
{
    emit buttonClicked();
}

/*
void ViewHeaderWithIcon::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
    setStyleName("CommonHeaderPanelClicked");
}

void ViewHeaderWithIcon::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
    setStyleName("CommonHeaderPanel");
    emit this->clicked();
}*/

void ViewHeaderWithIcon::setTitle(const QString &title)
{
    titleWidget->setText(title);
}
