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

#ifndef VIEWHEADERWITHICON_H
#define VIEWHEADERWITHICON_H

#include <MWidgetController>
#include <QGraphicsLinearLayout>
#include <MLabel>
#include <QObject>

class ViewHeaderWithIcon : public MWidgetController
{
    Q_OBJECT
public:
    ViewHeaderWithIcon(QGraphicsItem *parent = 0);

    void setTitle(const QString &title);

protected:
   // void mousePressEvent(QGraphicsSceneMouseEvent *event);
   // void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

signals:
   // void clicked();
    void buttonClicked();

private slots:
    void onButtonClicked();

private:
    QGraphicsLinearLayout *linearLayout;
    MLabel *titleWidget;

};

#endif // VIEWHEADERWITHICON_H
