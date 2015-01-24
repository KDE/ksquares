/***************************************************************************
 *   Copyright (C) 2006 by Matthew Williams    <matt@milliams.com>         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef KSQUARESWINDOW_H
#define KSQUARESWINDOW_H

#include <KXmlGuiWindow>

#include "ksquaresgame.h"
#include "ui_prefs_ai.h"
#include "ui_prefs_display.h"

class GameBoardView;
class GameBoardScene;
class QLabel;
/**
 * @short Mainwindow class
 *
 * Sets actions, controls the creation of the game class and the GameBoardScene
 *
 * @author Matt Williams <matt@milliams.com>
 */

class KSquaresWindow : public KXmlGuiWindow
{
	Q_OBJECT

	public:
		///Constructor
		KSquaresWindow();
		~KSquaresWindow();

	public slots:
		void  showHighscores();
		//void  configureHighscores();
		///Launch the new game dialog and create a new game
		void gameNew();
		///Start a new game with the same settings (read from KConfig Settings)
		void gameReset();

	private slots:
		void aiChooseLine(); // testing only
		void initObject();
		void optionsPreferences();
		void playerTakeTurn(KSquaresPlayer* currentPlayer);
		void gameOver(const QVector<KSquaresPlayer> &_playerList);	//when KSquaresGame says the game is over. Display score board

	private:

		//void setupAccel();
		void setupActions();
		Ui::prefs_ai ui_prefs_ai;
		Ui::prefs_display ui_prefs_display;
		///The game board view
		GameBoardView *m_view;
		///The game scene
		GameBoardScene *m_scene;
		///The game controller
		KSquaresGame* sGame;
		// Remember last network move
		int m_lastx1, m_lasty1, m_lastx2, m_lasty2;
                QLabel *m_player;
};

#endif // KSQUARESWINDOW_H
