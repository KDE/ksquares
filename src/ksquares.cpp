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
#include <QList>
#include <QRectF>

//kde
#include <KDE/KApplication>
#include <KDE/KConfigDialog>
#include <KDE/KStatusBar>
#include <KDE/KStandardAction>
#include <kdebug.h>
#include <KDE/KLocale>
#include <KDE/KCursor>
/*#include <khighscore.h>
#include <kexthighscore.h>*/
#include <kstandardgameaction.h>

//generated
#include "settings.h"

//classes
#include "aicontroller.h"
#include "gameboardview.h"

//ui
#include "newgamedialog.h"
#include "scoresdialog.h"

KSquares::KSquares() : KMainWindow(), m_view(new GameBoardView(this)), m_scene(0)
{	
	sGame = new KSquaresGame();
	connect(sGame, SIGNAL(takeTurnSig(KSquaresPlayer*)), this, SLOT(playerTakeTurn(KSquaresPlayer*)));
	connect(sGame, SIGNAL(gameOver(QVector<KSquaresPlayer>)), this, SLOT(gameOver(QVector<KSquaresPlayer>)));
	
	setCentralWidget(m_view);
	setupActions();
	statusBar()->insertPermanentItem(i18n("Current Player"), 0);
	statusBar()->show();
	setAutoSaveSettings();
}

/*KSquares::~KSquares()
{
}*/

void KSquares::setupActions()
{	
	KStandardGameAction::gameNew(this, SLOT(gameNew()), this);
	KStandardGameAction::quit(kapp, SLOT(quit()), this);
	KStandardAction::preferences(this, SLOT(optionsPreferences()), this);
	
	/*KStandardGameAction::highscores(this, SLOT(showHighscores()), actionCollection());
	KStandardGameAction::configureHighscores(this, SLOT(configureHighscores()), actionCollection());*/
	
	setupGUI();
}

/*void KSquares::configureHighscores() {KExtHighscore::configure(this);}
void KSquares::showHighscores() {KExtHighscore::show(this);}*/

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
		if (!((dialog.spinWidth->value() == 1) and (dialog.spinHeight->value() == 1)))
		{
			GameBoardScene* temp = m_scene;
			m_scene = new GameBoardScene(dialog.spinWidth->value(), dialog.spinHeight->value());
			m_scene->update();
			
			m_view->setScene(m_scene);
			m_view->setEnabled(true);
			delete temp;
			
			m_view->setBoardSize();	//refresh board zooming
			
			//start game etc.
			sGame->createGame(playerList, dialog.spinWidth->value(), dialog.spinHeight->value());
			connect(m_scene, SIGNAL(lineDrawn(int)), sGame, SLOT(addLineToIndex(int)));
			connect(sGame, SIGNAL(drawLine(int,QColor)), m_scene, SLOT(drawLine(int,QColor)));
			connect(sGame, SIGNAL(drawSquare(int,QColor)), m_scene, SLOT(drawSquare(int,QColor)));
		}
	}
}

void KSquares::gameOver(QVector<KSquaresPlayer> playerList)
{
	ScoresDialog scoresDialog(this);
	
	QStandardItemModel* scoreTableModel = new QStandardItemModel();
	scoreTableModel->setRowCount(playerList.size());
	scoreTableModel->setColumnCount(3);
	scoreTableModel->setHeaderData(0, Qt::Horizontal, i18n("Player Name"));
	scoreTableModel->setHeaderData(1, Qt::Horizontal, i18n("Score"));
	scoreTableModel->setHeaderData(2, Qt::Horizontal, i18n("Global Score"));
	
	qSort(playerList.begin(), playerList.end(), qGreater<KSquaresPlayer>());
	for(int i = 0; i <  playerList.size(); i++)
	{
		scoreTableModel->setItem(i, 0, new QStandardItem(playerList.at(i).name()));
		
		QString temp;
		temp.setNum(playerList.at(i).score());
		scoreTableModel->setItem(i, 1, new QStandardItem(temp));
		
		qreal score = qreal(playerList.at(i).score()) - ((qreal(Settings::boardWidth())*qreal(Settings::boardHeight())) / (playerList.size()));
		temp.setNum(score);
		scoreTableModel->setItem(i, 2, new QStandardItem(temp));
	}
	
	scoresDialog.scoreTable->setModel(scoreTableModel);
	//scoresDialog.scoreTable->adjustSize();
	
	scoresDialog.exec();
	
	if(playerList.at(0).isHuman())
	{
		/*KExtHighscore::Score score(KExtHighscore::Won);
		score.setScore(qreal(playerList.at(0).score()) - ((qreal(Settings::boardWidth())*qreal(Settings::boardHeight())) / (playerList.size())));
		KExtHighscore::submitScore(score, this);*/
	}
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
		
		setCursor(KCursor::arrowCursor());
		m_view->setEnabled(true);
	}
	else	//AI
	{
		//kDebug() << "AI's Turn" << endl;
		//lock the view to let the AI do it's magic
		setCursor(KCursor::waitCursor());
		m_view->setEnabled(false);
		
		aiController ai(sGame->currentPlayerId(), sGame->lines(), sGame->squares(), sGame->boardWidth(), sGame->boardHeight());
		//m_scene->addLineToIndex(ai.chooseLine());
		sGame->addLineToIndex(ai.chooseLine());
	}
}

#include "ksquares.moc"
