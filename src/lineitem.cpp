/***************************************************************************
 *   Copyright (C) 2006 by Matthew Williams    <matt@milliams.com>         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include <iostream>
using std::cout;
using std::endl;

#include "lineitem.h"

LineItem::LineItem(const QLineF &line, int ownerId, QGraphicsItem* parent, QGraphicsScene* scene) : QGraphicsLineItem(line, parent, scene), playerOwnerId(ownerId)
{
}
