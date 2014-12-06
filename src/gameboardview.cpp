/***************************************************************************
 *   Copyright (C) 2006 by Matthew Williams    <matt@milliams.com>         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "gameboardview.h"

GameBoardView::GameBoardView(QWidget *parent) : QGraphicsView(parent)
{
}

void GameBoardView::setBoardSize()
{
	if (scene() != 0)
		fitInView(scene()->sceneRect(), Qt::KeepAspectRatio);
}


