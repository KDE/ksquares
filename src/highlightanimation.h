/*
    SPDX-FileCopyrightText: 2006 Paolo Capriotti <p.capriotti@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

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
    explicit HighlightAnimation(const QLineF &line);
};

#endif // HIGHLIGHTANIMATION_H
