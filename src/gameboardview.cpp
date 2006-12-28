/***************************************************************************
 *   Copyright (C) 2006 by Matthew Williams    <matt@milliams.com>         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "settings.h"
#include "gameboardview.h"

GameBoardView::GameBoardView(QWidget *parent) : QGraphicsView(parent)
{
	gameScene = 0;
}

void GameBoardView::createBoard(int height, int width)
{
	if (gameScene)
	{
		//delete gameScene;
		gameScene = 0;
	}
	if ((height == 1) and (width == 1))
		return;
	gameScene = new GameBoardScene(height, width, this);
	setScene(gameScene);
	setBoardSize();
}

void GameBoardView::mouseMoveEvent(QMouseEvent* event)
{
	//cout << "GameBoardView::mouseMoveEvent" << endl;
	QGraphicsView::mouseMoveEvent(event);
}

void GameBoardView::resizeEvent(QResizeEvent* event)
{
	QGraphicsView::resizeEvent(event);
	setBoardSize();
}

void GameBoardView::setBoardSize()
{
	if(Settings::resizeBoard() == true)
	{
		if (scene() != 0)
			fitInView(scene()->sceneRect(), Qt::KeepAspectRatio);
	}
	else
	{
		resetMatrix();
	}
}

#include "gameboardview.moc"
