/*
    SPDX-FileCopyrightText: 2006 Matthew Williams <matt@milliams.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef THEMEGRAPHICSITEMS_H
#define THEMEGRAPHICSITEMS_H

#include <QGraphicsRectItem>

/**
 * @short Themable QGraphicsItem for completed squares
 *
 * @author Matt Williams <matt@milliams.com>
 */

class KSquaresSquareItem : public QGraphicsRectItem
{
public:
    explicit KSquaresSquareItem(QGraphicsItem *parent = nullptr);
    ~KSquaresSquareItem();

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) Q_DECL_OVERRIDE;
};

#endif // THEMEGRAPHICSITEMS_H
