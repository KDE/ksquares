/***************************************************************************
 *   Copyright (C) 2006 by Matthew Williams    <matt@milliams.com>         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef KSQUARES_H
#define KSQUARES_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <KDE/KMainWindow>

#include "ksquaresgame.h"
#include "ui_prefs_ai.h"
#include "ui_prefs_display.h"

class KToggleAction;
class GameBoardView;
class GameBoardScene;

/**
 * @short Mainwindow class
 *
 * Sets actions, controls the creation of the game class and the GameBoardScene
 * 
 * @author Matt Williams <matt@milliams.com>
 */

class KSquaresWindow : public KMainWindow
{
	Q_OBJECT
			
	public:
		///Constructor
		KSquaresWindow();
		//virtual ~KSquaresWindow();
	
	public slots:
		/*void  showHighscores();
		void  configureHighscores();*/
		///Launch the new game dialog and create a new game
		void gameNew();
	
	private slots:
		void optionsPreferences();
		void playerTakeTurn(KSquaresPlayer* currentPlayer);
		void gameOver(QVector<KSquaresPlayer> playerList);	//when KSquaresGame says the game is over. Display score board
	
	private:
		void setupAccel();
		void setupActions();
		Ui::prefs_ai ui_prefs_ai;
		Ui::prefs_display ui_prefs_display;
		///The game board view
		GameBoardView *m_view;
		///The game scene
		GameBoardScene *m_scene;
		///The game controller
		KSquaresGame* sGame;
		
		//KToggleAction *m_toolbarAction;
		//KToggleAction *m_statusbarAction;
};

#endif // KSQUARES_H
