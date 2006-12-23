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
#include "settings.h"
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
#include "gameboardscene.h"

#include "newgamedialog.h"
#include "scoresdialog.h"

KSquares::KSquares() : KMainWindow(), m_view(new GameBoardView(this))
{	
	kdDebug() << "4" << endl;
	sGame = new KSquaresGame();
	//connect(m_view, SIGNAL(gameStarted()), sGame, SLOT(startGame()));
	connect(sGame, SIGNAL(playerChangedSig(int)), this, SLOT(playerChanged(int)));
	setCentralWidget(m_view);
	setupActions();
	statusBar()->insertPermanentItem("Player: ", 0);
	statusBar()->show();
	setAutoSaveSettings();
}

KSquares::~KSquares()
{
}

void KSquares::setupActions()
{	
	KStandardAction::openNew(this, SLOT(fileNew()), actionCollection());
	KStandardAction::quit(kapp, SLOT(quit()), actionCollection());
	KStandardAction::preferences(this, SLOT(optionsPreferences()), actionCollection());
	
	// custom menu and menu item - the slot is in the class KSquaresView
	KAction *custom = new KAction(i18n("Swi&tch Colors"), actionCollection(), "switch_action");
	custom->setIcon(KIcon("colorize"));
	setupGUI();
}

void KSquares::fileNew()
{
	//load settings
	NewGameDialog dialog(this);
	dialog.spinNumOfPlayers->setValue(Settings::numOfPlayers());
	dialog.playerOneName->setText(Settings::playerNames().at(0));
	dialog.playerTwoName->setText(Settings::playerNames().at(1));
	dialog.spinHeight->setValue(Settings::boardHeight());
	dialog.spinWidth->setValue(Settings::boardWidth());
	
	//run dialog
	dialog.exec();
	
	//save settings
	Settings::setNumOfPlayers(dialog.spinNumOfPlayers->value());
	
	QStringList tempNames;
	tempNames.append(dialog.playerOneName->text());
	tempNames.append(dialog.playerTwoName->text());
	Settings::setPlayerNames(tempNames);
	
	//Settings::setPlayerOneName(dialog.playerOneName->text());
	//Settings::setPlayerTwoName(dialog.playerTwoName->text());
	Settings::setBoardHeight(dialog.spinHeight->value());
	Settings::setBoardWidth(dialog.spinWidth->value());
	Settings::writeConfig();
	
	QVector<KSquaresPlayer> playerList;
	playerList.append(KSquaresPlayer(dialog.playerOneName->text(), true));
	playerList.append(KSquaresPlayer(dialog.playerTwoName->text(), true));
	
	m_view->createBoard(Settings::boardWidth(), Settings::boardHeight());
	
	//start game etc.
	//sGame->createGame(dialog.spinNumOfPlayers->value(), Settings::boardWidth(), Settings::boardHeight());
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
		scoreTableModel->setItem(i, 0, new QStandardItem(Settings::playerNames().at(i)));
		
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
	QWidget *generalSettingsDlg = new QWidget;
	ui_prefs_base.setupUi(generalSettingsDlg);
	dialog->addPage(generalSettingsDlg, i18n("General"), "package_setting");
	//connect(dialog, SIGNAL(settingsChanged(const QString &)), m_view, SLOT(settingsChanged()));
	dialog->show();
}

void KSquares::playerChanged(int playerNumber)
{	
	//QString temp;
	//temp.setNum(playerNumber);
	statusBar()->changeItem(Settings::playerNames().at(playerNumber), 0);
}

#include "ksquares.moc"
