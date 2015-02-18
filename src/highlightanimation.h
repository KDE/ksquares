/***************************************************************************
 *   Copyright (C) 2006 by Paolo Capriotti    <p.capriotti@gmail.com>      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef HIGHLIGHTANIMATION_H
#define HIGHLIGHTANIMATION_H

#include <QGraphicsLineItem>

/**
 * @short Highlight animation when new lines appear.
 *
 * Created when a new line is added to the board.
 *
 * @author Paolo Capriotti <p.capriotti@gmail.com>
 */
class HighlightAnimation : public QObject, public QGraphicsLineItem
{
    Q_OBJECT
    Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity)
public:
    HighlightAnimation(const QLineF &line);
};

#endif // HIGHLIGHTANIMATION_H
