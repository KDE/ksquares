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
#include "dots_client.h"

class KToggleAction;
class GameBoardView;
class GameBoardScene;
class KGGZRankingsDialog;
class KGGZSeatsDialog;

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
		void slotNetworkData(int fd);
		void slotNetworkError();
		void slotNetworkPacket(dotsOpcodes::Opcode opcode, const msg& message);
		void slotMoveRequest(int x1, int y1, int x2, int y2);
		void slotRankingsRequest();
		void slotSeatsRequest();

	private:
		enum StatusBarItem
		{
			statusplayer,
			statusnetwork
		};

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
		///Network protocol handler
		dots *m_proto;
		// Remember last network move
		int m_lastx1, m_lasty1, m_lastx2, m_lasty2;
		//Network scores and player list
		KGGZRankingsDialog *m_rankingsdlg;
		KGGZSeatsDialog *m_seatsdlg;

		//KToggleAction *m_toolbarAction;
		//KToggleAction *m_statusbarAction;
};

#endif // KSQUARESWINDOW_H
