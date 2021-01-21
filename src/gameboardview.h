/*
    SPDX-FileCopyrightText: 2006 Matthew Williams <matt@milliams.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

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
    explicit GameBoardView(QWidget *parent = nullptr);
    /**
     * Asks the scene through ( qobject_cast<GameBoardScene*>(scene()) ) for the smallest size it should be
     */
    QSize minimumSizeHint() const override
    {
        if (scene() == nullptr) {
            return QSize();
        } else {
            return qobject_cast<GameBoardScene *>(scene())->minimumSizeHint();
        }
    }
public Q_SLOTS:
    ///Automatically resizes the board according to the users preferences. Deprecated for a bit
    void setBoardSize();
protected:
    ///Overloaded to resize board.
    void resizeEvent(QResizeEvent *event) override
    {
        if (scene() != nullptr) {
            fitInView(scene()->sceneRect(), Qt::KeepAspectRatio);
        } QGraphicsView::resizeEvent(event);
    }
};

#endif // GAMEBOARDVIEW_H
