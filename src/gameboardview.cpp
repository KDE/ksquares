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

#include "gameboardview.h"

GameBoardView::GameBoardView(QWidget *parent) : QGraphicsView(parent)
{
	gameScene = 0;
}

void GameBoardView::createBoard(int height, int width)
{
	/*if (gameScene)
		delete gameScene;*/
	if ((height == 1) and (width == 1))
		return;
	gameScene = new GameBoardScene(height, width, this);
	setScene(gameScene);
	emit gameStarted();
}

void GameBoardView::mouseMoveEvent(QMouseEvent* e)
{
	//cout << "GameBoardView::mouseMoveEvent" << endl;
	QGraphicsView::mouseMoveEvent(e);
}

#include "gameboardview.moc"
