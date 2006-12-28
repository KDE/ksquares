/***************************************************************************
 *   Copyright (C) 2006 by Matthew Williams    <matt@milliams.com>         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include <math.h>

#include <QGraphicsSceneMouseEvent>

#include <kdebug.h>

#include "settings.h"
#include "gameboardscene.h"

GameBoardScene::GameBoardScene(int newWidth, int newHeight, QObject *parent) : QGraphicsScene(parent), width(newWidth), height(newHeight), lineDrawn((2*newWidth*newHeight + newWidth + newHeight), false)
{
	//setObjectName("GameBoardScene");
	
	spacing = 40;
	for(int iWidth = 0; iWidth <= width; iWidth++)
	{
		for(int iHeight = 0; iHeight <= height; iHeight++)
		{
			int x = iWidth*spacing;
			int y = iHeight*spacing;
			addEllipse(QRectF(-1,-1,2,2), QPen(Qt::SolidLine), QBrush(Qt::SolidPattern))->setPos(x,y);
		}
	}
	QPen feintPen(Qt::DotLine);
	feintPen.setColor(QColor(0,0,0,31));
	for(int iWidth = 0; iWidth <= width; iWidth++)
	{
		addLine(QLineF(spacing*iWidth, 0, spacing*iWidth, spacing*height), feintPen);
	}
	for(int iHeight = 0; iHeight <= height; iHeight++)
	{
		addLine(QLineF(0, spacing*iHeight, spacing*width, spacing*iHeight), feintPen);
	}
	
	indicatorLine = new QGraphicsLineItem(spacing, spacing, spacing, spacing);
	addItem(indicatorLine);
	
	//lineDrawn.resize(2*width*height + width + height);	//now done in constructor
	squareOwnerTable.fill(-1, (width*height));
	
	QGraphicsEllipseItem tempItem;
	QGraphicsEllipseItemType = tempItem.type();
}

bool GameBoardScene::isLineAlready(QList<QGraphicsEllipseItem*> pointPair)
{
	int index = -1;
	if (pointPair.size() == 2)	// if it really is a pair
	{
		qreal pointOneX = pointPair.at(0)->scenePos().x()/spacing;
		qreal pointOneY = pointPair.at(0)->scenePos().y()/spacing;
		qreal pointTwoX = pointPair.at(1)->scenePos().x()/spacing;
		qreal pointTwoY = pointPair.at(1)->scenePos().y()/spacing;
		
		//this int conversion could go bad but SHOULD be safe
		qreal refX;	// these two will be the grid-coord of the
		qreal refY;	// to and left most point of the two
		
		enum{HORZONTAL, VERTICAL} dir;
		if (pointOneX == pointTwoX)
		{
			dir = VERTICAL;
			refX = pointOneX;
			if (pointTwoY < pointOneY)	//want the topmost point as reference
				refY = pointTwoY;
			else
				refY = pointOneY;
		}
		else if (pointOneY == pointTwoY)
		{
			dir = HORZONTAL;
			refY = pointOneY;
			if (pointOneX < pointTwoX)	//want the leftmost point as reference
				refX = pointOneX;
			else
				refX = pointTwoX;
		}
		
		if (dir == HORZONTAL)
		{
			index = refY * ((2*width)+1) + refX;
		}
		else if (dir == VERTICAL)
		{
			index = refY * ((2*width)+1) + refX + width;
		}
	}
	if (index == -1)
		return true;
	
	return lineDrawn.at(index);
}

bool GameBoardScene::addLineToIndex(QList<QGraphicsEllipseItem*> pointPair)
{
	int index = -1;
	if (pointPair.size() == 2)	// if it really is a pair
	{
		qreal pointOneX = pointPair.at(0)->scenePos().x()/spacing;
		qreal pointOneY = pointPair.at(0)->scenePos().y()/spacing;
		qreal pointTwoX = pointPair.at(1)->scenePos().x()/spacing;
		qreal pointTwoY = pointPair.at(1)->scenePos().y()/spacing;
		
		//this int conversion could go bad but SHOULD be safe
		qreal refX;	// these two will be the grid-coord of the
		qreal refY;	// to and left most point of the two
		
		enum{HORZONTAL, VERTICAL} dir;
		if (pointOneX == pointTwoX)
		{
			dir = VERTICAL;
			refX = pointOneX;
			if (pointTwoY < pointOneY)	//want the topmost point as reference
				refY = pointTwoY;
			else
				refY = pointOneY;
		}
		else if (pointOneY == pointTwoY)
		{
			dir = HORZONTAL;
			refY = pointOneY;
			if (pointOneX < pointTwoX)	//want the leftmost point as reference
				refX = pointOneX;
			else
				refX = pointTwoX;
		}
		
		if (dir == HORZONTAL)
		{
			index = refY * ((2*width)+1) + refX;
		}
		else if (dir == VERTICAL)
		{
			index = refY * ((2*width)+1) + refX + width;
		}
	}
	if (index == -1)	//not a valid line since no two unique ends
		return false;
	
	return addLineToIndex(index);
}

bool GameBoardScene::addLineToIndex(int index)
{
	//kDebug() << "Adding line at index " << index << endl;
	if (lineDrawn[index] == false)	//if there's not a line there yet, do checks
	{
		lineDrawn[index] = true;
		
		QGraphicsLineItem* line = lineFromIndex(index);
		line->setPen(QPen(QBrush(Settings::lineColor(), Qt::SolidPattern), 2.5));
		addItem(line);
		
		checkForNewSquares();
		return true;
	}
	else	//else wait for another line to be drawn
	{
		return false;
	}
}

QGraphicsLineItem* GameBoardScene::lineFromIndex(int index)
{
	int index2 = index % ((2*width) + 1);
	enum{HORZONTAL, VERTICAL} dir;
	if(index2 < width)
		dir = HORZONTAL;
	else
		dir = VERTICAL;
	
	int yCoordStart = (index / ((2*width) + 1)) * spacing;
	int xCoordStart = 0;
	int yCoordEnd = 0;
	int xCoordEnd = 0;
	switch(dir)
	{
		case HORZONTAL:
			xCoordStart = index2 * spacing;
			yCoordEnd = yCoordStart;
			xCoordEnd = xCoordStart + spacing;
			break;
		case VERTICAL:
			xCoordStart = (index2 - width) * spacing;
			yCoordEnd = yCoordStart + spacing;
			xCoordEnd = xCoordStart;
			break;
	}
	return new QGraphicsLineItem(QLineF(xCoordStart, yCoordStart, xCoordEnd, yCoordEnd));
}

void GameBoardScene::checkForNewSquares()
{	
	for(int i=0; i < (width*height); i++)	//cycle through every box..
	{
		if (squareOwnerTable.at(i) == -1)	//..checking it if there is no current owner
		{
			//indices of the lines surrounding the box; Correlates to "QVector<bool> lineDrawn"
			int index1 = (i/width) * ((2*width) + 1) + (i%width);
			int index2 = index1 + width;
			int index3 = index2 + 1;
			int index4 = index3 + width;
			//cout << index1 << ", " << index2 << ", " << index3 << ", " << index4 << " - " << lineDrawn.size() << endl;
			if (lineDrawn.at(index1) and lineDrawn.at(index2) and lineDrawn.at(index3) and lineDrawn.at(index4))
			{
				//kdDebug() << "Square " << i << " completed." << endl;
				emit squareComplete(i);
			}
		}
	}
	emit lineDrawnSig();
}

void GameBoardScene::setSquareOwner(int squareIndex, int owner)
{
	squareOwnerTable[squareIndex] = owner;	//TODO out of bounds crash (squareIndex=4)
	drawSquare(squareIndex);
}

void GameBoardScene::drawSquare(int index)
{
	QBrush brush(Qt::SolidPattern);
	switch(squareOwnerTable[index])
	{
		case -1:
			brush.setColor(Qt::white);	//owner -1 is not a valid player
			break;
		case 0:
			brush.setColor(Qt::red);	
			break;
		case 1:
			brush.setColor(Qt::blue);
			break;
		case 2:
			brush.setColor(Qt::green);
			break;
		case 3:
			brush.setColor(Qt::yellow);
			break;
		default:
			brush.setColor(Qt::black);	//this probably means there is something wrong (or more than 4 player :S)
	}
	
	addRect(QRectF(qreal((index%width)*spacing), qreal((index/width)*spacing), qreal(spacing), qreal(spacing)), QPen(), brush)->setZValue(-1);
}

QSize GameBoardScene::sizeHint()
{
	return QSize(width*spacing, height*spacing);
}

void GameBoardScene::mouseDoubleClickEvent (QGraphicsSceneMouseEvent* mouseEvent)
{
	//cout << "GameBoardScene::mouseDoubleClickEvent" << endl;
	QGraphicsScene::mouseDoubleClickEvent(mouseEvent);
}

void GameBoardScene::mousePressEvent (QGraphicsSceneMouseEvent* mouseEvent)
{
	//cout << "GameBoardScene::mousePressEvent" << endl;
	buttonPress = mouseEvent->buttons();	//store the buttton press for mouseReleaseEvent()
	QGraphicsScene::mousePressEvent(mouseEvent);
}

void GameBoardScene::mouseReleaseEvent (QGraphicsSceneMouseEvent* mouseEvent)
{
	//cout << "GameBoardScene::mouseReleaseEvent" << endl;
	if (buttonPress == Qt::LeftButton)
	{
		QList<QGraphicsEllipseItem*> connectList = getTwoNearestPoints(mouseEvent->scenePos());
		if (connectList.size() == 2)
		{
			addLineToIndex(connectList);
			/*if (addLineToIndex(connectList) == true)	//try to add the line to the index
			{
				indicatorLine->setPen(QPen(QBrush(QColor(255,255,0,0), Qt::SolidPattern), 2.0));	//just make the pen invisible
				//QGraphicsLineItem* newLine = new QGraphicsLineItem(QLineF(connectList.at(0)->scenePos(), connectList.at(1)->scenePos()));
				//newLine->setPen(QPen(QBrush(QColor(0,0,0), Qt::SolidPattern), 2.5));
				//addItem(newLine);
			}*/
		}
	}
	
	QGraphicsScene::mouseReleaseEvent(mouseEvent);
}

void GameBoardScene::mouseMoveEvent (QGraphicsSceneMouseEvent* mouseEvent)
{
	//indicatorLine = 0;
	
	//cout << "GameBoardScene::mouseMoveEvent" << endl;
	//cout << "mouseEvent->scenePos(): " << mouseEvent->scenePos().x() << ", " << mouseEvent->scenePos().y() << endl;
	
	QList<QGraphicsEllipseItem*> connectList = getTwoNearestPoints(mouseEvent->scenePos());
	
	if (connectList.size() == 2)
	{
		if (not isLineAlready(connectList))	// if there is not already a line there
		{
			indicatorLine->setLine(QLineF(connectList.at(0)->scenePos(), connectList.at(1)->scenePos()));
			indicatorLine->setPen(QPen(QBrush(Settings::indicatorLineColor(), Qt::SolidPattern), 2.0));
		}
		else
		{
			indicatorLine->setPen(QPen(QBrush(Settings::indicatorLineColor(), Qt::SolidPattern), 2.0));
		}
		
	}
	else
	{
		indicatorLine->setPen(QPen(QBrush(Settings::indicatorLineColor(), Qt::SolidPattern), 2.0));
	}
	
	QGraphicsScene::mouseMoveEvent(mouseEvent);
}

QList<QGraphicsEllipseItem*> GameBoardScene::getTwoNearestPoints(QPointF pos) const
{
	QList<QGraphicsItem *> itemList = items();
	QList<QGraphicsEllipseItem*> connectList;
	for (int i = 0; i < itemList.size(); ++i) 
	{
		if (itemList.at(i)->type() == QGraphicsEllipseItemType)
		{
			//cout << "itemList.at(i)->scenePos(): " << qgraphicsitem_cast<QGraphicsEllipseItem*>(itemList.at(i))->scenePos().x() << ", " << qgraphicsitem_cast<QGraphicsEllipseItem*>(itemList.at(i))->scenePos().y() << endl;
			QPointF dist(pos - itemList.at(i)->scenePos());
			qreal distMod = sqrt(dist.x()*dist.x() + dist.y()*dist.y());
			//if (distMod < (spacing*0.7071))	//there will only ever be either 1 or 2 items that fulfil this [0.7071 ~ 2^(-0.5)]
			if (distMod < spacing-5)
			{
				connectList << qgraphicsitem_cast<QGraphicsEllipseItem*>(itemList.at(i));
			}
		}
	}
	return connectList;
}

#include "gameboardscene.moc"
