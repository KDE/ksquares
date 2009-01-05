/***************************************************************************
 *   Copyright (C) 2006 by Matthew Williams    <matt@milliams.com>         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "themegraphicsitems.h"

#include "settings.h"

KSquaresSquareItem::KSquaresSquareItem(QGraphicsItem* parent) : QGraphicsRectItem(parent)
{
}

KSquaresSquareItem::~KSquaresSquareItem(){}

void KSquaresSquareItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	QGraphicsRectItem::paint(painter, option, widget);
}

