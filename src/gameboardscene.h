/***************************************************************************
 *   Copyright (C) 2006 by Matthew Williams    <matt@milliams.com>         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef GAMEBOARDSCENE_H
#define GAMEBOARDSCENE_H

#include <QGraphicsScene>
#include <QGraphicsEllipseItem>

/**
 * @short Scene for displaying the game board
 *
 * Created anew at the beginning of each game.
 * Contains QGraphicsLineItems for lines, QGraphicsEllipseItems for the dots and QGraphicsRectItem for when a square is complete.
 * 
 * @author Matt Williams <matt@milliams.com>
 */

class GameBoardScene : public QGraphicsScene
{
	Q_OBJECT
	
	public:
		GameBoardScene(int newWidth, int newHeight, QObject *parent = 0);
		~GameBoardScene();
		const QSize minimumSizeHint() const;	//I'm not sure if this is even used?
		
	public slots:
		void drawLine(int index, QColor colour);
		void drawSquare(int index, QColor colour);
		
	protected:
		QList<QGraphicsEllipseItem*> getTwoNearestPoints(QPointF pos) const;
		
		bool isLineAlready(QList<QGraphicsEllipseItem*> pointPair) const;
		void addLineToIndex(QList<QGraphicsEllipseItem*> pointPair);
		
		//conversion functions
		int indexFromPointPair(QList<QGraphicsEllipseItem*> pointPair) const;	//given a pointPair, returns the index of the line between them. If not a valid line, returns -1
		QGraphicsLineItem* lineFromIndex(int index) const;	//all external calls will need to be passed through this to convert to local coords
		
		Qt::MouseButtons buttonPress;	//just a temporary store - needed since buttons() in mouseReleaseEvent() will be zero since there are no buttons currently pressed (they've just been released :S)
		
		QGraphicsLineItem* indicatorLine;
		
		QList<bool> lineList;	//non-canon. Just kept in sync with the KSquaresGame one
		
		int QGraphicsEllipseItemType;
		int width;
		int height;
		int spacing;
		
		//event handlers
		void mousePressEvent (QGraphicsSceneMouseEvent* mouseEvent);
		void mouseReleaseEvent (QGraphicsSceneMouseEvent* mouseEvent);
		void mouseMoveEvent (QGraphicsSceneMouseEvent* mouseEvent);
		
	signals:
		/**
		 * Emitted when a click is encountered that relates to an as-yet undrawn line
		 */
		void lineDrawn(int);
};

#endif // GAMEBOARDSCENE_H
