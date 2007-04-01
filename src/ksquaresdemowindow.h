/***************************************************************************
 *   Copyright (C) 2006 by Matthew Williams    <matt@milliams.com>         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef KSQUARESDEMOWINDOW_H
#define KSQUARESDEMOWINDOW_H

#include <KMainWindow>

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

class KSquaresDemoWindow : public KMainWindow
{
	Q_OBJECT
			
	public:
		///Constructor
		KSquaresDemoWindow();
	
	public slots:
		void gameNew();
	
	private slots:
		void aiChooseLine();
		void playerTakeTurn(KSquaresPlayer* currentPlayer);
		void gameOver(QVector<KSquaresPlayer> /*playerList*/);
	
	private:
		///The game board view
		GameBoardView *m_view;
		///The game scene
		GameBoardScene *m_scene;
		///The game controller
		KSquaresGame* sGame;
};

#endif // KSQUARESDEMOWINDOW_H
