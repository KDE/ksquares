/***************************************************************************
 *   Copyright (C) 2006 by Paolo Capriotti    <p.capriotti@gmail.com>      *
 *   Copyright (C) 2010 by Stefan Majewsky    <majewsky@gmx.net>           *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "highlightanimation.h"

#include "settings.h"

#include <QPen>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>

HighlightAnimation::HighlightAnimation(const QLineF &line)
	: QGraphicsLineItem(line)
{
	setPen(QPen(Settings::highlightColor(), 8.0, Qt::SolidLine, Qt::RoundCap));

	QPropertyAnimation* animation = new QPropertyAnimation(this, "opacity", this);
	animation->setStartValue(1.0);
	animation->setEndValue(0.0);
	QSequentialAnimationGroup* animGroup = new QSequentialAnimationGroup(this);
	animGroup->addPause(1000);
	animGroup->addAnimation(animation);
	animGroup->start(QAbstractAnimation::DeleteWhenStopped);
	connect(animGroup, &QSequentialAnimationGroup::finished, this, &HighlightAnimation::deleteLater);
}


