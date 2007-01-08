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
		
		//getters
		QList<bool> lines() const {return lineList;}
		int boardWidth() const {return width;}
		int boardHeight() const {return height;}
		
	public slots:
		void drawLine(int index, QColor colour);
		void drawSquare(int index, QColor colour);
		
	protected:
		QList<QGraphicsEllipseItem*> getTwoNearestPoints(QPointF pos) const;
		
		bool isLineAlready(QList<QGraphicsEllipseItem*> pointPair);
		void addLineToIndex(QList<QGraphicsEllipseItem*> pointPair);	//returns true if line is new and was added
		
		//conversion functions
		int indexFromPointPair(QList<QGraphicsEllipseItem*> pointPair);	//given a pointPair, returns the index of the line between them. If not a valid line, returns -1
		QGraphicsLineItem* lineFromIndex(int index);	//all external calls will need to be passed through this to convert to local coords
		
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
		void lineDrawn(int);
};

#endif // GAMEBOARDSCENE_H
