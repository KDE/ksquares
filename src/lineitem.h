/***************************************************************************
 *   Copyright (C) 2006 by Matthew Williams    <matt@milliams.com>         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef LINEITEM_H
#define LINEITEM_H

#include <QGraphicsLineItem>

class LineItem : public QGraphicsLineItem
{
	public:
		LineItem(const QLineF & line, int ownerId, QGraphicsItem* parent = 0, QGraphicsScene* scene = 0);
		
		enum { Type = UserType + 1 };
		int type() const {return Type;}
		int playerOwner() const {return playerOwnerId;}
	protected:
		int playerOwnerId;
};

#endif // LINEITEM_H
