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

#include <kapplication.h>
#include <kmainwindow.h>

#include "gameboardview.h"
#include "ksquaresgame.h"
//#include "ui_prefs_base.h"
#include "ui_prefs_ai.h"
#include "ui_prefs_display.h"

class KToggleAction;

class KSquares : public KMainWindow
{
	Q_OBJECT
			
	public:
		KSquares();
		virtual ~KSquares();
	
	private slots:
		void gameNew();
		void optionsPreferences();
		void playerChanged(KSquaresPlayer currentPlayer);
		void gameOver(QVector<KSquaresPlayer> playerList);	//when KSquaresGame says the game is over. Display score board
	
	private:
		void setupAccel();
		void setupActions();
		//Ui::prefs_base ui_prefs_base;
		Ui::prefs_ai ui_prefs_ai;
		Ui::prefs_display ui_prefs_display;
		GameBoardView *m_view;
		KSquaresGame* sGame;
		
		//KPrinter   *m_printer;
		KToggleAction *m_toolbarAction;
		KToggleAction *m_statusbarAction;
};

#endif // KSQUARES_H
