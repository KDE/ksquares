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
#include <QGraphicsEllipseItem>

#include <kdebug.h>

#include "settings.h"
#include "gameboardscene.h"

GameBoardScene::GameBoardScene(int newWidth, int newHeight, QObject *parent) : QGraphicsScene(parent), width(newWidth), height(newHeight)
{
	kDebug() << "GameBoardScene::GameBoardScene()" << endl;
	
	for(int i = 0; i < (2*width*height + width + height); i++)
	{
		lineList.append(false);
	}
	
	spacing = 40;	//this hard coding doesn't matter since auto-resizing exists :)
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
	
	QGraphicsEllipseItem tempItem;
	QGraphicsEllipseItemType = tempItem.type();
	//QGraphicsEllipseItemType = QGraphicsEllipseItem::type();
	
	qreal border = 10;
	QRectF rect = sceneRect();
	rect.setLeft(rect.left() - border);
	rect.setRight(rect.right() + border);
	rect.setTop(rect.top() - border);
	rect.setBottom(rect.bottom() + border);
	setSceneRect(rect);
}

GameBoardScene::~GameBoardScene()
{
	kDebug() << "GameBoardScene::~GameBoardScene()" << endl;
}

void GameBoardScene::drawLine(int index, QColor colour)
{
	QGraphicsLineItem* line = lineFromIndex(index);
	line->setPen(QPen(QBrush(colour), 2.5));
	addItem(line);	//draw new line
	lineList[index] = true;	//keep this table in sync
	indicatorLine->setPen(QPen(QBrush(Qt::transparent), 2.5));	//make indicator transparrent
	update(line->boundingRect());
}

void GameBoardScene::drawSquare(int index, QColor colour)
{
	QBrush brush(colour, Qt::SolidPattern);
	
	addRect(QRectF(qreal((index%width)*spacing), qreal((index/width)*spacing), qreal(spacing), qreal(spacing)), QPen(), brush)->setZValue(-1);
}

int GameBoardScene::indexFromPointPair(QList<QGraphicsEllipseItem*> pointPair) const
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
		
		if (pointOneX == pointTwoX)
		{
			//dir = VERTICAL;
			refX = pointOneX;
			if (pointTwoY < pointOneY)	//want the topmost point as reference
				refY = pointTwoY;
			else
				refY = pointOneY;
			index = refY * ((2*width)+1) + refX + width;
		}
		else if (pointOneY == pointTwoY)
		{
			//dir = HORIZONTAL;
			refY = pointOneY;
			if (pointOneX < pointTwoX)	//want the leftmost point as reference
				refX = pointOneX;
			else
				refX = pointTwoX;
			index = refY * ((2*width)+1) + refX;
		}
	}
	return index;
}

QGraphicsLineItem* GameBoardScene::lineFromIndex(int index) const
{
	int index2 = index % ((2*width) + 1);
	enum{HORIZONTAL, VERTICAL} dir;
	if(index2 < width)
		dir = HORIZONTAL;
	else
		dir = VERTICAL;
	
	int yCoordStart = (index / ((2*width) + 1)) * spacing;
	int xCoordStart = 0;
	int yCoordEnd = 0;
	int xCoordEnd = 0;
	switch(dir)
	{
		case HORIZONTAL:
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

bool GameBoardScene::isLineAlready(QList<QGraphicsEllipseItem*> pointPair) const
{
	int index = indexFromPointPair(pointPair);
	if (index == -1)
		return true;
	
	return lineList.at(index);
}

void GameBoardScene::addLineToIndex(QList<QGraphicsEllipseItem*> pointPair)
{
	int index = indexFromPointPair(pointPair);
	if (index == -1)	//not a valid line since no two unique ends
		return;
	
	emit lineDrawn(index);	//addLineToIndex(index);
}

QList<QGraphicsEllipseItem*> GameBoardScene::getTwoNearestPoints(QPointF pos) const
{
	QList<QGraphicsItem*> itemList = items();
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

const QSize GameBoardScene::minimumSizeHint() const
{
	return QSize((width*spacing)+10, (height*spacing)+10);	// the +10 is to provide padding and to avoid scrollbars
}

void GameBoardScene::mousePressEvent (QGraphicsSceneMouseEvent* mouseEvent)
{
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
		}
	}
	
	QGraphicsScene::mouseReleaseEvent(mouseEvent);
}

void GameBoardScene::mouseMoveEvent (QGraphicsSceneMouseEvent* mouseEvent)
{
	//indicatorLine = 0;
	
	//kDebug() << "GameBoardScene::mouseMoveEvent" << endl;
	//kDebug() << "mouseEvent->scenePos(): " << mouseEvent->scenePos().x() << ", " << mouseEvent->scenePos().y() << endl;
	
	QList<QGraphicsEllipseItem*> connectList = getTwoNearestPoints(mouseEvent->scenePos());
	
	if (connectList.size() == 2)	//if there are two nearest points
	{
		if (not isLineAlready(connectList))	//only two nearest points && no line there yet
		{
			indicatorLine->setLine(QLineF(connectList.at(0)->scenePos(), connectList.at(1)->scenePos()));	//where
			indicatorLine->setPen(QPen(QBrush(Settings::indicatorLineColor()), 2.5));	//draw visible
		}
		else	//only two nearest points && already a line there
		{
			indicatorLine->setPen(QPen(QBrush(Qt::transparent), 2.5));	//draw invisible
		}
		
	}
	else
	{
		indicatorLine->setPen(QPen(QBrush(Qt::transparent), 2.5));	//draw invisible
	}
	
	QGraphicsScene::mouseMoveEvent(mouseEvent);
}

#include "gameboardscene.moc"
