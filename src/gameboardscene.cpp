/*
    SPDX-FileCopyrightText: 2006 Matthew Williams <matt@milliams.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "gameboardscene.h"

#include "highlightanimation.h"

#include <cmath>

#include <QGraphicsSceneMouseEvent>
#include <QGraphicsEllipseItem>

#include <QDebug>

GameBoardScene::GameBoardScene(int newWidth, int newHeight, QObject *parent) : QGraphicsScene(parent), width(newWidth), height(newHeight), acceptEvents(true)
{
    ////qDebug() << "GameBoardScene::GameBoardScene()";

    for (int i = 0; i < (2 * width * height + width + height); i++) {
        lineList.append(false); //simply fill array with 'false's
    }

    spacing = 40;   //this hard coding doesn't matter since auto-resizing exists :)
    for (int iWidth = 0; iWidth <= width; iWidth++) {
        for (int iHeight = 0; iHeight <= height; iHeight++) {
            int x = iWidth * spacing;
            int y = iHeight * spacing;
            QGraphicsEllipseItem *dot = new QGraphicsEllipseItem(QRectF(-2, -2, 4, 4));
            dot->moveBy(x, y);
            dot->setBrush(Qt::SolidPattern);
            dot->setZValue(20); // set the elevation, the dot's are on top
            addItem(dot);
        }
    }
    QPen feintPen(Qt::DotLine); //for the guidelines between dots
    feintPen.setWidth(1);
    feintPen.setColor(Qt::lightGray);
    for (int iWidth = 0; iWidth <= width; iWidth++) {
        addLine(QLineF(spacing * iWidth, 0, spacing * iWidth, spacing * height), feintPen);
    }
    for (int iHeight = 0; iHeight <= height; iHeight++) {
        addLine(QLineF(0, spacing * iHeight, spacing * width, spacing * iHeight), feintPen);
    }

    setBackgroundBrush(QBrush(Qt::white));

    indicatorLine = new QGraphicsLineItem(1, 1, 1, 1);
    indicatorLine->setZValue(10);
    indicatorLine->setPen(QPen(Qt::yellow, 2.5));
    indicatorLine->hide();
    addItem(indicatorLine);

    QGraphicsEllipseItem tempItem;
    QGraphicsEllipseItemType = tempItem.type();

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
    //qDebug() << "GameBoardScene::~GameBoardScene()";
    delete indicatorLine;
}

void GameBoardScene::drawLine(int index, const QColor &colour)
{
    QGraphicsLineItem *line = new QGraphicsLineItem(lineFromIndex(index));
    line->setZValue(10);
    line->setPen(QPen(QBrush(colour), 2.5));
    addItem(line);  //draw new line
    lineList[index] = true; //keep this table in sync
    indicatorLine->hide();
    update(line->boundingRect());
}

void GameBoardScene::highlightLine(int index)
{
    HighlightAnimation *anim = new HighlightAnimation(lineFromIndex(index));
    anim->setZValue(9);
    addItem(anim);
}

void GameBoardScene::drawSquare(int index, const QColor &colour)
{
    QBrush brush(colour, Qt::SolidPattern);

    addRect(QRectF(qreal((index % width)*spacing), qreal((index / width)*spacing), qreal(spacing), qreal(spacing)), QPen(), brush)->setZValue(-1);
}

int GameBoardScene::indexFromPointPair(const QList<QGraphicsEllipseItem *> &pointPair) const
{

    if (pointPair.size() != 2) {
        return -1;    // if it isn't a pair
    }

    qreal pointOneX = pointPair.at(0)->scenePos().x() / spacing;
    qreal pointOneY = pointPair.at(0)->scenePos().y() / spacing;
    qreal pointTwoX = pointPair.at(1)->scenePos().x() / spacing;
    qreal pointTwoY = pointPair.at(1)->scenePos().y() / spacing;

    //this int conversion could go bad but SHOULD be safe
    qreal refX; // these two will be the grid-coord of the
    qreal refY; // to and left most point of the two

    int index = -1;
    if (pointOneX == pointTwoX) {
        //dir = VERTICAL;
        refX = pointOneX;
        if (pointTwoY < pointOneY) { //want the topmost point as reference
            refY = pointTwoY;
        } else {
            refY = pointOneY;
        }
        index = static_cast<int>(refY * ((2 * width) + 1) + refX + width);
    }

    else if (pointOneY == pointTwoY) {
        //dir = HORIZONTAL;
        refY = pointOneY;
        if (pointOneX < pointTwoX) { //want the leftmost point as reference
            refX = pointOneX;
        } else {
            refX = pointTwoX;
        }
        index = static_cast<int>(refY * ((2 * width) + 1) + refX);
    }
    return index;
}

QLineF GameBoardScene::lineFromIndex(int index) const
{
    int index2 = index % ((2 * width) + 1);
    enum {HORIZONTAL, VERTICAL} dir;
    if (index2 < width) {
        dir = HORIZONTAL;
    } else {
        dir = VERTICAL;
    }

    int yCoordStart = (index / ((2 * width) + 1)) * spacing;
    int xCoordStart = 0;
    int yCoordEnd = 0;
    int xCoordEnd = 0;
    switch (dir) {
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
    return QLineF(xCoordStart, yCoordStart, xCoordEnd, yCoordEnd);
}

bool GameBoardScene::isLineAlready(const QList<QGraphicsEllipseItem *> &pointPair) const //TODO does this work?
{
    int index = indexFromPointPair(pointPair);
    if (index == -1) {
        return true;
    }

    return lineList.at(index);
}

void GameBoardScene::addLineToIndex(const QList<QGraphicsEllipseItem *> &pointPair)
{
    int index = indexFromPointPair(pointPair);
    if (index == -1) {  //not a valid line since no two unique ends
        return;
    }

    Q_EMIT lineDrawn(index);  //addLineToIndex(index);
}

QList<QGraphicsEllipseItem *> GameBoardScene::getTwoNearestPoints(const QPointF &pos) const
{
    QList<QGraphicsItem *> itemList = items();
    QList<QGraphicsEllipseItem *> connectList;
    for (int i = 0; i < itemList.size(); ++i) {
        if (itemList.at(i)->type() == QGraphicsEllipseItemType) {
            //cout << "itemList.at(i)->scenePos():" << qgraphicsitem_cast<QGraphicsEllipseItem*>(itemList.at(i))->scenePos().x() << "," << qgraphicsitem_cast<QGraphicsEllipseItem*>(itemList.at(i))->scenePos().y() << endl;
            QPointF dist(pos - itemList.at(i)->scenePos());
            qreal distMod = sqrt(dist.x() * dist.x() + dist.y() * dist.y());
            //if (distMod < (spacing*0.7071))   //there will only ever be either 1 or 2 items that fulfil this [0.7071 ~ 2^(-0.5)]
            if (distMod < spacing - 5) {
                connectList << qgraphicsitem_cast<QGraphicsEllipseItem *>(itemList.at(i));
            }
        }
    }
    return connectList;
}

const QSize GameBoardScene::minimumSizeHint() const
{
    return QSize((width * spacing) + 10, (height * spacing) + 10); // the +10 is to provide padding and to avoid scrollbars
}

/*void GameBoardScene::mousePressEvent (QGraphicsSceneMouseEvent* mouseEvent)
{
    if (!acceptEvents) return;
    QGraphicsScene::mousePressEvent(mouseEvent);
}*/

void GameBoardScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (!acceptEvents) {
        QGraphicsScene::mouseReleaseEvent(mouseEvent);
        return;
    }

    //cout << "GameBoardScene::mouseReleaseEvent" << endl;
    if (mouseEvent->button() == Qt::LeftButton) {
        QList<QGraphicsEllipseItem *> connectList = getTwoNearestPoints(mouseEvent->scenePos());
        if (connectList.size() == 2) {
            addLineToIndex(connectList);
        }
    }

    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}

void GameBoardScene::acknowledgeMove(int x1, int y1, int x2, int y2)
{
    QPoint calculatedpos(((x1 + x2) / 2.0) * spacing, ((y1 + y2) / 2.0) * spacing);
    QList<QGraphicsEllipseItem *> connectList = getTwoNearestPoints(calculatedpos);
    addLineToIndex(connectList);
}

void GameBoardScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (!acceptEvents) {
        return;
    }
    //indicatorLine = 0;

    ////qDebug() << "GameBoardScene::mouseMoveEvent";
    ////qDebug() << "mouseEvent->scenePos():" << mouseEvent->scenePos().x() << "," << mouseEvent->scenePos().y();

    QList<QGraphicsEllipseItem *> connectList = getTwoNearestPoints(mouseEvent->scenePos());

    if (connectList.size() == 2 && !isLineAlready(connectList)) {
        //if there are two nearest points and there isn't already a line
        indicatorLine->setLine(QLineF(connectList.at(0)->scenePos(), connectList.at(1)->scenePos()));   //where
        indicatorLine->show();
    } else {
        indicatorLine->hide();
    }

    QGraphicsScene::mouseMoveEvent(mouseEvent);
}

