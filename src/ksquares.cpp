/***************************************************************************
 *   Copyright (C) 2006 by Matthew Williams    <matt@milliams.com>         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include <iostream>
using std::cout;
using std::endl;

#include "ksquares.h"

#include <kconfigdialog.h>
#include <kglobal.h>
#include <klocale.h>
#include <kicon.h>
#include <kstatusbar.h>
#include <kkeydialog.h>
#include <kedittoolbar.h>
#include <kstdaccel.h>
#include <kaction.h>
#include <kstandardaction.h>
#include <kstdgameaction.h>
#include <kdebug.h>

#include "settings.h"
#include "aicontroller.h"

#include "gameboardscene.h"
#include "newgamedialog.h"
#include "scoresdialog.h"

KSquares::KSquares() : KMainWindow(), m_view(new GameBoardView(this))
{	
	sGame = new KSquaresGame();
	//connect(m_view, SIGNAL(gameStarted()), sGame, SLOT(startGame()));
	connect(sGame, SIGNAL(playerChangedSig(KSquaresPlayer)), this, SLOT(playerChanged(KSquaresPlayer)));
	setCentralWidget(m_view);
	setupActions();
	statusBar()->insertPermanentItem(i18n("Current Player"), 0);
	statusBar()->show();
	setAutoSaveSettings();
	
	//gameNew();	//uncomment to start a new game on startup
}

KSquares::~KSquares()
{
}

void KSquares::setupActions()
{	
	//KStdGameAction::gameNew(this, SLOT(gameNew()), actionCollection());
	//KStdGameAction::quit(kapp, SLOT(quit()), actionCollection());
	KStandardAction::openNew(this, SLOT(gameNew()), actionCollection());
	KStandardAction::quit(kapp, SLOT(quit()), actionCollection());
	KStandardAction::preferences(this, SLOT(optionsPreferences()), actionCollection());
	
	// custom menu and menu item - the slot is in the class KSquaresView
	//KAction *custom = new KAction(i18n("Swi&tch Colors"), actionCollection(), "switch_action");
	//custom->setIcon(KIcon("colorize"));
	setupGUI();
}

void KSquares::gameNew()
{
	//load settings
	NewGameDialog dialog(this);
	dialog.spinNumOfPlayers->setValue(Settings::numOfPlayers());
	dialog.playerOneName->setText(Settings::playerNames().at(0));
	dialog.playerTwoName->setText(Settings::playerNames().at(1));
	dialog.spinHeight->setValue(Settings::boardHeight());
	dialog.spinWidth->setValue(Settings::boardWidth());
	if (Settings::humanList().at(0) == 2)
		dialog.playerOneHuman->setCheckState(Qt::Checked);
	else
		dialog.playerOneHuman->setCheckState(Qt::Unchecked);
	if (Settings::humanList().at(1) == 2)
		dialog.playerTwoHuman->setCheckState(Qt::Checked);
	else
		dialog.playerTwoHuman->setCheckState(Qt::Unchecked);
	
	//run dialog
	dialog.exec();
	
	//save settings
	Settings::setNumOfPlayers(dialog.spinNumOfPlayers->value());
	
	QStringList tempNames;
	tempNames.append(dialog.playerOneName->text());
	tempNames.append(dialog.playerTwoName->text());
	Settings::setPlayerNames(tempNames);
	
	QList<int> tempHuman;
	tempHuman.append(dialog.playerOneHuman->checkState());
	tempHuman.append(dialog.playerTwoHuman->checkState());
	Settings::setHumanList(tempHuman);
	
	Settings::setBoardHeight(dialog.spinHeight->value());
	Settings::setBoardWidth(dialog.spinWidth->value());
	Settings::writeConfig();
	
	//create players
	QVector<KSquaresPlayer> playerList;
	switch(dialog.playerOneHuman->checkState())
	{
		case 0:
			playerList.append(KSquaresPlayer(dialog.playerOneName->text(), false));
			break;
		case 2:
			playerList.append(KSquaresPlayer(dialog.playerOneName->text(), true));
			break;
	}
	switch(dialog.playerTwoHuman->checkState())
	{
		case 0:
			playerList.append(KSquaresPlayer(dialog.playerTwoName->text(), false));
			break;
		case 2:
			playerList.append(KSquaresPlayer(dialog.playerTwoName->text(), true));
			break;
	}
	
	//create phsical board
	m_view->createBoard(Settings::boardWidth(), Settings::boardHeight());
	
	//start game etc.
	sGame->createGame(playerList, Settings::boardWidth(), Settings::boardHeight());
	sGame->startGame();
	
	// From here on out, the game is 'controlled' by GameBoardScene and the clicks therein
	//cout << "Connecting stuff" << endl;	
	connect(m_view->scene(), SIGNAL(squareComplete(int)), sGame, SLOT(playerSquareComplete(int)));
	connect(m_view->scene(), SIGNAL(lineDrawnSig()), sGame, SLOT(tryEndGo()));
	connect(sGame, SIGNAL(setSquareOwnerSig(int,int)), m_view->scene(), SLOT(setSquareOwner(int,int)));
	connect(sGame, SIGNAL(gameOverSig(QVector<KSquaresPlayer>)), this, SLOT(gameOver(QVector<KSquaresPlayer>)));
}

void KSquares::gameOver(QVector<KSquaresPlayer> playerList)
{
	ScoresDialog scoresDialog(this);
	
	QStandardItemModel* scoreTableModel = new QStandardItemModel();
	scoreTableModel->setRowCount(playerList.size());
	scoreTableModel->setColumnCount(2);
	
	for(int i = 0; i <  playerList.size(); i++)
	{
		scoreTableModel->setItem(i, 0, new QStandardItem(playerList.at(i).name()));
		
		QString temp;
		temp.setNum(playerList.at(i).score());
		scoreTableModel->setItem(i, 1, new QStandardItem(temp));
	}
	
	scoresDialog.scoreTable->setModel(scoreTableModel);
	
	scoresDialog.exec();
}

void KSquares::optionsPreferences()
{
	KConfigDialog *dialog = new KConfigDialog(this, "settings", Settings::self());
	
	QWidget *displaySettingsDlg = new QWidget;
	ui_prefs_display.setupUi(displaySettingsDlg);
	dialog->addPage(displaySettingsDlg, i18n("Display"), "ksquares_display");
	
	QWidget *aiSettingsDlg = new QWidget;
	ui_prefs_ai.setupUi(aiSettingsDlg);
	dialog->addPage(aiSettingsDlg, i18n("AI"), "ksquares_ai");
	
	connect(dialog, SIGNAL(settingsChanged(const QString &)), m_view, SLOT(setBoardSize()));
	dialog->show();
}

void KSquares::playerChanged(KSquaresPlayer currentPlayer)
{	
	statusBar()->changeItem(currentPlayer.name(), 0);
	if(currentPlayer.isHuman())
	{
		//kdDebug() << "Humans's Turn" << endl;
		//Let the human player interact with the board through the GameBoardView
		if(!m_view->isEnabled())
			m_view->setEnabled(true);
	}
	else	//AI
	{
		//kdDebug() << "AI's Turn" << endl;
		//lock the view to let the AI do it's magic
		if(m_view->isEnabled())
			m_view->setEnabled(false);
		
		
	}
}

#include "ksquares.moc"
