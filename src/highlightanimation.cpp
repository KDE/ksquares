/*
    SPDX-FileCopyrightText: 2006 Paolo Capriotti <p.capriotti@gmail.com>
    SPDX-FileCopyrightText: 2010 Stefan Majewsky <majewsky@gmx.net>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "highlightanimation.h"

#include "settings.h"

#include <QPen>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>

HighlightAnimation::HighlightAnimation(const QLineF &line)
    : QGraphicsLineItem(line)
{
    setPen(QPen(Settings::highlightColor(), 8.0, Qt::SolidLine, Qt::RoundCap));

    QPropertyAnimation *animation = new QPropertyAnimation(this, "opacity", this);
    animation->setStartValue(1.0);
    animation->setEndValue(0.0);
    QSequentialAnimationGroup *animGroup = new QSequentialAnimationGroup(this);
    animGroup->addPause(1000);
    animGroup->addAnimation(animation);
    animGroup->start(QAbstractAnimation::DeleteWhenStopped);
    connect(animGroup, &QSequentialAnimationGroup::finished, this, &HighlightAnimation::deleteLater);
}

