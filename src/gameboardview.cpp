/*
    SPDX-FileCopyrightText: 2006 Matthew Williams <matt@milliams.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "gameboardview.h"

GameBoardView::GameBoardView(QWidget *parent) : QGraphicsView(parent)
{
}

void GameBoardView::setBoardSize()
{
    if (scene() != nullptr) {
        fitInView(scene()->sceneRect(), Qt::KeepAspectRatio);
    }
}

