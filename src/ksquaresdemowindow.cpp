/***************************************************************************
 *   Copyright (C) 2006 by Matthew Williams    <matt@milliams.com>         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

//header
#include "ksquaresdemowindow.h"

//qt
#include <QTimer>

//kde
#include <KApplication>
#include <KStatusBar>
#include <KActionCollection>
#include <kdebug.h>
#include <KLocale>
#include <kstandardgameaction.h>

//classes
#include "aicontroller.h"
#include "gameboardview.h"

KSquaresDemoWindow::KSquaresDemoWindow() : KXmlGuiWindow(), m_view(new GameBoardView(this)), m_scene(0)
{
	sGame = new KSquaresGame();
	connect(sGame, SIGNAL(takeTurnSig(KSquaresPlayer*)), this, SLOT(playerTakeTurn(KSquaresPlayer*)));
	connect(sGame, SIGNAL(gameOver(QVector<KSquaresPlayer>)), this, SLOT(gameOver(QVector<KSquaresPlayer>)));

	m_view->setRenderHints(QPainter::Antialiasing);
	m_view->setFrameStyle(QFrame::NoFrame);
	m_view->setDisabled(true);
	setCentralWidget(m_view);

	KStandardGameAction::quit(kapp, SLOT(quit()), actionCollection());
	setupGUI();

	statusBar()->insertPermanentItem(i18n("Current Player"), 0);
	statusBar()->show();
}

void KSquaresDemoWindow::gameNew()
{
	//create players
	QVector<KSquaresPlayer> playerList;
	for(int i=0; i<4; i++)
	{
		QColor color;
		switch(i)
		{
			case 0:
				color = Qt::red;
				break;
			case 1:
				color = Qt::blue;
				break;
			case 2:
				color = Qt::green;
				break;
			case 3:
				color = Qt::yellow;
				break;
			default:
				kError() << "KSquaresGame::playerSquareComplete(); currentPlayerId() != 0|1|2|3";
		}
		playerList.append(KSquaresPlayer(i18n("Player %1", i+1), color, false));
	}

	//create physical board
	GameBoardScene* temp = m_scene;
	m_scene = new GameBoardScene(15, 10);

	m_view->setScene(m_scene);
	delete temp;

	m_view->setBoardSize();	//refresh board zooming

	//start game etc.
	sGame->createGame(playerList, 15, 10);
	connect(m_scene, SIGNAL(lineDrawn(int)), sGame, SLOT(addLineToIndex(int)));
	connect(sGame, SIGNAL(drawLine(int,QColor)), m_scene, SLOT(drawLine(int,QColor)));
	connect(sGame, SIGNAL(drawSquare(int,QColor)), m_scene, SLOT(drawSquare(int,QColor)));

	sGame->start();
}

void KSquaresDemoWindow::playerTakeTurn(KSquaresPlayer* currentPlayer)
{
	statusBar()->changeItem(currentPlayer->name(), 0);
	QTimer::singleShot(200, this, SLOT(aiChooseLine()));
}

void KSquaresDemoWindow::aiChooseLine()
{
	aiController ai(sGame->currentPlayerId(), sGame->lines(), sGame->squares(), sGame->boardWidth(), sGame->boardHeight());
	sGame->addLineToIndex(ai.chooseLine());
}

void KSquaresDemoWindow::gameOver(const QVector<KSquaresPlayer> & /*playerList*/)
{
	kDebug() << "Game Over";
	QTimer::singleShot(1000, this, SLOT(gameNew()));
}

#include "ksquaresdemowindow.moc"
