/***************************************************************************
 *   Copyright (C) 2006 by Matthew Williams    <matt@milliams.com>         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef GAMEBOARDVIEW_H
#define GAMEBOARDVIEW_H

#include <QGraphicsView>

#include "gameboardscene.h"

/**
 * @short Actual game board widget
 *
 * A simple subclass of QGraphicsView simply to be able to resize the board according to user preferences.
 *
 * @author Matt Williams <matt@milliams.com>
 */

class GameBoardView : public QGraphicsView
{
    Q_OBJECT

public:
    /**
     * Constructor
     *
     * @param parent the parent of the widget
     */
    explicit GameBoardView(QWidget *parent = 0);
    /**
     * Asks the scene through ( qobject_cast<GameBoardScene*>(scene()) ) for the smallest size it should be
     */
    QSize minimumSizeHint() const Q_DECL_OVERRIDE
    {
        if (scene() == 0) {
            return QSize();
        } else {
            return qobject_cast<GameBoardScene *>(scene())->minimumSizeHint();
        }
    }
public slots:
    ///Automatically resizes the board according to the users preferences. Deprecated for a bit
    void setBoardSize();
protected:
    ///Overloaded to resize board.
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE
    {
        if (scene() != 0) {
            fitInView(scene()->sceneRect(), Qt::KeepAspectRatio);
        } QGraphicsView::resizeEvent(event);
    }
};

#endif // GAMEBOARDVIEW_H
