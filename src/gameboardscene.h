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

//This class' public API should be reduced to simply 'add a line at index i' and 'colour square i <red>'
//It shouldnt store the game board (except maybe for convenience) as the canon game board should be in the KSquaresGame class

class GameBoardScene : public QGraphicsScene
{
	Q_OBJECT
	
	public:
		GameBoardScene(int newWidth, int newHeight, QObject *parent = 0);
		~GameBoardScene();
		QSize sizeHint();
		QList<int> squareOwners() const {return squareOwnerTable;}	//should be in game class
		QList<bool> lines() const {return lineDrawn;}	//should be in game class
		bool addLineToIndex(int index);	//should be in game class and replaced with an addLineToScene() fn
		int boardWidth() const {return width;}	//should be in game class
		int boardHeight() const {return height;}	//should be in game class
		
	public slots:
		void setSquareOwner(int squareIndex, int owner);
		
	protected:
		QList<QGraphicsEllipseItem*> getTwoNearestPoints(QPointF pos) const;
		
		void mousePressEvent (QGraphicsSceneMouseEvent* mouseEvent);
		void mouseReleaseEvent (QGraphicsSceneMouseEvent* mouseEvent);
		void mouseMoveEvent (QGraphicsSceneMouseEvent* mouseEvent);
		
		bool isLineAlready(QList<QGraphicsEllipseItem*> pointPair);
		bool addLineToIndex(QList<QGraphicsEllipseItem*> pointPair);	//returns true if line is new and was added
		void checkForNewSquares();
		
		int indexFromPointPair(QList<QGraphicsEllipseItem*> pointPair);	//given a pointPair, returns the index of the line between them. If not a valid line, returns -1
		
		QGraphicsLineItem* lineFromIndex(int index);
		
		Qt::MouseButtons buttonPress;
		
		QGraphicsLineItem* indicatorLine;
		
		QList<int> squareOwnerTable;	// Along top row, then 2nd row et cetera. Size = width*height
		
		// In this order: top row of horizontal lines, first row of vertical lines, 2nd row of horizontal lines etc...
		// Size: 2*width*height + width + height
		QList<bool> lineDrawn;
		
		int QGraphicsEllipseItemType;	//keep
		int width;	//move
		int height;	//move
		int spacing;	//move
		
	protected slots:
		void drawSquare(int index);
		
	signals:
		void squareComplete(int);	// int is the index associated with 'QVector<int> squareOwnerTable'
		void lineDrawnSig();
};

#endif // GAMEBOARDSCENE_H
