/***************************************************************************
 *   Copyright (C) 2006 by Matthew Williams    <matt@milliams.com>         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

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
		KSquaresSquareItem(QGraphicsItem* parent=0);
		~KSquaresSquareItem();
		
		void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget=0);
};

#endif // THEMEGRAPHICSITEMS_H
