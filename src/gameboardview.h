/***************************************************************************
 *   Copyright (C) 2006 by Matthew Williams    <matt@milliams.com>         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef GAMEBOARDVIEW_H
#define GAMEBOARDVIEW_H

#include <QGraphicsView>

#include "gameboardscene.h"

/**
 * @short Displays the game board
 *
 * A simple subclass of QGraphicsView simply to be able to resize the board according to user preferences.
 * 
 * @author Matt Williams <matt@milliams.com>
 */

class GameBoardView : public QGraphicsView
{
	Q_OBJECT
	
	public:
		GameBoardView(QWidget *parent = 0);
		QSize sizeHint() {return qobject_cast<GameBoardScene*>(scene())->sizeHint();}
	public slots:
		void setBoardSize();
	protected:
		void resizeEvent(QResizeEvent* event) {setBoardSize(); QGraphicsView::resizeEvent(event);}
};

#endif // GAMEBOARDVIEW_H
