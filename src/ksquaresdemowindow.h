/*
    SPDX-FileCopyrightText: 2006 Matthew Williams <matt@milliams.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef KSQUARESDEMOWINDOW_H
#define KSQUARESDEMOWINDOW_H

#include <KXmlGuiWindow>

#include "ksquaresgame.h"

class GameBoardView;
class GameBoardScene;

/**
 * @short Mainwindow class for demonstration mode
 *
 * Creates a very simple mainwindow and continuously plays 4 computer players against eachother.
 *
 * @author Matt Williams <matt@milliams.com>
 */

class KSquaresDemoWindow : public KXmlGuiWindow
{
    Q_OBJECT

public:
    ///Constructor
    KSquaresDemoWindow();

public Q_SLOTS:
    void gameNew();

private Q_SLOTS:
    void aiChooseLine();
    void playerTakeTurn(KSquaresPlayer *currentPlayer);
    void gameOver(const QVector<KSquaresPlayer> & /*playerList*/);

private:
    ///The game board view
    GameBoardView *m_view;
    ///The game scene
    GameBoardScene *m_scene;
    ///The game controller
    KSquaresGame *sGame;
};

#endif // KSQUARESDEMOWINDOW_H
