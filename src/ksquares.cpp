/***************************************************************************
 *   Copyright (C) 2006 by Matthew Williams    <matt@milliams.com>         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

//header
#include "ksquares.h"

//qt
#include <QStandardItemModel>

//kde
#include <kapplication.h>
#include <kconfigdialog.h>
#include <kstatusbar.h>
#include <kstandardaction.h>
#include <kstandardgameaction.h>
#include <kdebug.h>
#include <klocale.h>

//generated
#include "settings.h"

//classes
#include "aicontroller.h"
#include "gameboardscene.h"

//ui
#include "newgamedialog.h"
#include "scoresdialog.h"

KSquares::KSquares() : KMainWindow(), m_view(new GameBoardView(this))
{	
	sGame = new KSquaresGame();
	//connect(m_view, SIGNAL(gameStarted()), sGame, SLOT(startGame()));
	connect(sGame, SIGNAL(takeTurnSig(KSquaresPlayer*)), this, SLOT(playerTakeTurn(KSquaresPlayer*)));
	connect(sGame, SIGNAL(gameOverSig(QVector<KSquaresPlayer>)), this, SLOT(gameOver(QVector<KSquaresPlayer>)));
	
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
	//KStandardGameAction::gameNew(this, SLOT(gameNew()), actionCollection());
	//KStandardGameAction::quit(kapp, SLOT(quit()), actionCollection());
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
	
	//create indexed arrays of the widgets
	QList<QLineEdit*> nameLineEditList;
	nameLineEditList.append(dialog.playerOneName);
	nameLineEditList.append(dialog.playerTwoName);
	nameLineEditList.append(dialog.playerThreeName);
	nameLineEditList.append(dialog.playerFourName);
	QList<QCheckBox*> humanCheckBoxList;
	humanCheckBoxList.append(dialog.playerOneHuman);
	humanCheckBoxList.append(dialog.playerTwoHuman);
	humanCheckBoxList.append(dialog.playerThreeHuman);
	humanCheckBoxList.append(dialog.playerFourHuman);
	
	//get settings from file
	for(int i=0; i<=3; i++)
	{
		nameLineEditList.at(i)->setText(Settings::playerNames().at(i));
	}
	for(int i=0; i<=3; i++)
	{
		if (Settings::humanList().at(i) == 2)
			humanCheckBoxList.at(i)->setCheckState(Qt::Checked);
		else
			humanCheckBoxList.at(i)->setCheckState(Qt::Unchecked);
	}
	dialog.spinNumOfPlayers->setValue(Settings::numOfPlayers());
	dialog.spinHeight->setValue(Settings::boardHeight());
	dialog.spinWidth->setValue(Settings::boardWidth());
	
	//run dialog
	dialog.exec();
	
	if (dialog.result() == QDialog::Accepted)
	{
		//save settings
		Settings::setNumOfPlayers(dialog.spinNumOfPlayers->value());
		
		QStringList tempNames;
		for(int i=0; i<=3; i++)
		{
			tempNames.append(nameLineEditList.at(i)->text());
		}
		Settings::setPlayerNames(tempNames);
		
		QList<int> tempHuman;
		for(int i=0; i<=3; i++)
		{
			tempHuman.append(humanCheckBoxList.at(i)->checkState());
		}
		Settings::setHumanList(tempHuman);
		
		Settings::setBoardHeight(dialog.spinHeight->value());
		Settings::setBoardWidth(dialog.spinWidth->value());
		Settings::writeConfig();
		
		//create players
		QVector<KSquaresPlayer> playerList;
		for(int i=0; i<dialog.spinNumOfPlayers->value(); i++)
		{
			playerList.append(KSquaresPlayer(nameLineEditList.at(i)->text(), humanCheckBoxList.at(i)->isChecked()));
		}
		
		//create physical board
		m_view->createBoard(dialog.spinWidth->value(), dialog.spinHeight->value());
		//m_view->setEnabled(true);
		
		//start game etc.
		sGame->createGame(playerList, dialog.spinWidth->value(), dialog.spinHeight->value());
		sGame->startGame();
		
		connect(m_view->scene(), SIGNAL(lineDrawnSig()), sGame, SLOT(tryEndGo()));
		connect(m_view->scene(), SIGNAL(squareComplete(int)), sGame, SLOT(playerSquareComplete(int)));
		connect(sGame, SIGNAL(setSquareOwnerSig(int,int)), m_view->scene(), SLOT(setSquareOwner(int,int)));
	}
}

void KSquares::gameOver(QVector<KSquaresPlayer> playerList)
{
	ScoresDialog scoresDialog(this);
	
	QStandardItemModel* scoreTableModel = new QStandardItemModel();
	scoreTableModel->setRowCount(playerList.size());
	scoreTableModel->setColumnCount(2);
	scoreTableModel->setHeaderData(0, Qt::Horizontal, i18n("Player Name"));
	scoreTableModel->setHeaderData(1, Qt::Horizontal, i18n("Score"));
	
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

void KSquares::playerTakeTurn(KSquaresPlayer* currentPlayer)
{
	//kDebug() << "void KSquares::playerTakeTurn(KSquaresPlayer* currentPlayer)" << endl;
	statusBar()->changeItem(currentPlayer->name(), 0);
	if(currentPlayer->isHuman())
	{
		//kDebug() << "Humans's Turn" << endl;
		//Let the human player interact with the board through the GameBoardView
		m_view->setEnabled(true);
	}
	else	//AI
	{
		//kDebug() << "AI's Turn" << endl;
		//lock the view to let the AI do it's magic
		m_view->setEnabled(false);
		
		aiController ai(sGame->currentPlayerId(), m_view->scene()->lines(), m_view->scene()->squareOwners(), m_view->scene()->boardWidth(), m_view->scene()->boardHeight());
		m_view->scene()->addLineToIndex(ai.drawLine());
	}
}

#include "ksquares.moc"
