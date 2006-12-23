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

#include <QtGui>

class GameBoardScene : public QGraphicsScene
{
	Q_OBJECT
	
	public:
		GameBoardScene(int newWidth, int newHeight, QWidget *parent = 0);
		QSize sizeHint();
		QVector<int> board() const {return squareOwnerTable;}
	public slots:
		void setSquareOwner(int squareIndex, int owner);
		void drawSquare(int index);
	protected:
		QList<QGraphicsEllipseItem*> getTwoNearestPoints(QPointF pos) const;
		
		void mousePressEvent (QGraphicsSceneMouseEvent* mouseEvent);
		void mouseReleaseEvent (QGraphicsSceneMouseEvent* mouseEvent);
		void mouseDoubleClickEvent (QGraphicsSceneMouseEvent* mouseEvent);
		void mouseMoveEvent (QGraphicsSceneMouseEvent* mouseEvent);
		int width;
		int height;
		int spacing;
		
		bool isLineAlready(QList<QGraphicsEllipseItem*> pointPair);
		bool addLineToIndex(QList<QGraphicsEllipseItem*> pointPair);	//returns true if line is new and was added
		void checkForNewSquares();
		
		Qt::MouseButtons buttonPress;
		
		QGraphicsLineItem* indicatorLine;
		
		QVector<int> squareOwnerTable;	// Along top row, then 2nd row et cetera.
		
		// In this order: top row of horizontal lines, first row of vertical lines, 2nd row of horizontal lines etc...
		// Size: 2*width*height + width + height
		QVector<bool> lineDrawn;
		
		int QGraphicsEllipseItemType;
		
	signals:
		void squareComplete(int);	// int is the index associated with 'QVector<int> squareOwnerTable'
		void lineDrawnSig();
};

#endif // GAMEBOARDSCENE_H
