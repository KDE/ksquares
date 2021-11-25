/*
    SPDX-FileCopyrightText: 2006 Matthew Williams <matt@milliams.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "themegraphicsitems.h"

#include "settings.h"

KSquaresSquareItem::KSquaresSquareItem(QGraphicsItem *parent) : QGraphicsRectItem(parent)
{
}

KSquaresSquareItem::~KSquaresSquareItem() = default;

void KSquaresSquareItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QGraphicsRectItem::paint(painter, option, widget);
}

