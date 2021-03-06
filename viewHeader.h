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

#ifndef HEADER_H
#define HEADER_H

#include <MWidgetController>
#include <QGraphicsLinearLayout>
#include <MLabel>

class ViewHeader : public MWidgetController
{
public:
    ViewHeader(QGraphicsItem *parent = 0) :
        MWidgetController(parent),
        linearLayout(0),
        titleWidget(0)
    {
        setStyleName("CommonHeaderPanel");
        setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        linearLayout = new QGraphicsLinearLayout(Qt::Horizontal, this);
        titleWidget = new MLabel();
        titleWidget->setTextElide(true);
        titleWidget->setStyleName("CommonHeaderInverted");
        linearLayout->addItem(titleWidget);
    }

    void setTitle(const QString &title)
    {
        titleWidget->setText(title);
    }

private:
    QGraphicsLinearLayout *linearLayout;
    MLabel *titleWidget;

};

#endif // HEADER_H
