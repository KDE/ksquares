/***************************************************************************
 *   Copyright (C) 2006 by Paolo Capriotti    <p.capriotti@gmail.com>      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "highlightanimation.h"

#include "settings.h"

#include <QBrush>
#include <QGraphicsScene>
#include <QPen>
#include <QTimer>

HighlightAnimation::HighlightAnimation(const QLineF &line) : QGraphicsLineItem(line), timeline(1000)
{
	setPen(QPen(Settings::highlightColor(), 8.0, Qt::SolidLine, Qt::RoundCap));
	timeline.setUpdateInterval(10);
	timeline.setFrameRange(255, 0);
	connect(&timeline, SIGNAL(frameChanged(int)), this, SLOT(setOpacity(int)));
	connect(&timeline, SIGNAL(finished()), this, SLOT(removeMe()));
	QTimer::singleShot(1000, &timeline, SLOT(start()));
}

void HighlightAnimation::setOpacity(int opacity)
{
	QPen p = pen();
	QColor c = p.color();
	c.setAlpha(opacity);
	p.setColor(c);
	setPen(p);
	
	update();
}

void HighlightAnimation::removeMe()
{
	scene()->removeItem(this); // This is needed because otherwise
	                           // the scene gets a full repaint on removal
	deleteLater();
}

#include "highlightanimation.moc"
