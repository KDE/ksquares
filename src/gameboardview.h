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
		/**
		 * Constructor
		 * 
		 * @param parent the parent of the widget
		 */
		GameBoardView(QWidget* parent = 0);
		/**
		 * Asks the scene through ( qobject_cast<GameBoardScene*>(scene()) ) for the smallest size it should be
		 */
		QSize minimumSizeHint() const {if(scene()==0) return QSize(); else return qobject_cast<GameBoardScene*>(scene())->minimumSizeHint();}
	public slots:
		///Automatically resizes the board according to the users preferences.
		void setBoardSize();
	protected:
		///Overloaded to provide capability to resize board as the user wants.
		void resizeEvent(QResizeEvent* event) {setBoardSize(); QGraphicsView::resizeEvent(event);}
};

#endif // GAMEBOARDVIEW_H
