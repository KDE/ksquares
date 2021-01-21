/*
    SPDX-FileCopyrightText: 2006 Matthew Williams <matt@milliams.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

//header
#include "ksquaresdemowindow.h"

//qt
#include <QTimer>

//kde
#include <QApplication>
#include <QStatusBar>
#include <KActionCollection>
#include <QDebug>
#include <KLocalizedString>
#include <kstandardgameaction.h>

//classes
#include "aicontroller.h"
#include "gameboardview.h"

KSquaresDemoWindow::KSquaresDemoWindow() : KXmlGuiWindow(), m_view(new GameBoardView(this)), m_scene(nullptr)
{
    sGame = new KSquaresGame();
    connect(sGame, &KSquaresGame::takeTurnSig, this, &KSquaresDemoWindow::playerTakeTurn);
    connect(sGame, &KSquaresGame::gameOver, this, &KSquaresDemoWindow::gameOver);

    m_view->setRenderHints(QPainter::Antialiasing);
    m_view->setFrameStyle(QFrame::NoFrame);
    m_view->setDisabled(true);
    setCentralWidget(m_view);

    KStandardGameAction::quit(qApp, &QApplication::quit, actionCollection());
    setupGUI();

    //QT5 statusBar()->insertPermanentItem(i18n("Current Player"), 0);
    statusBar()->show();
}

void KSquaresDemoWindow::gameNew()
{
    //create players
    QVector<KSquaresPlayer> playerList;
    for (int i = 0; i < 4; i++) {
        QColor color;
        switch (i) {
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
            qCritical() << "KSquaresGame::playerSquareComplete(); currentPlayerId() != 0|1|2|3";
        }
        playerList.append(KSquaresPlayer(i18n("Player %1", i + 1), color, false));
    }

    //create physical board
    GameBoardScene *temp = m_scene;
    m_scene = new GameBoardScene(15, 10);

    m_view->setScene(m_scene);
    delete temp;

    m_view->setBoardSize(); //refresh board zooming

    //start game etc.
    sGame->createGame(playerList, 15, 10);
    connect(m_scene, &GameBoardScene::lineDrawn, sGame, &KSquaresGame::addLineToIndex);
    connect(sGame, &KSquaresGame::drawLine, m_scene, &GameBoardScene::drawLine);
    connect(sGame, &KSquaresGame::highlightMove, m_scene, &GameBoardScene::highlightLine);
    connect(sGame, &KSquaresGame::drawSquare, m_scene, &GameBoardScene::drawSquare);

    sGame->start();
}

void KSquaresDemoWindow::playerTakeTurn(KSquaresPlayer *currentPlayer)
{
    //QT5 statusBar()->changeItem(currentPlayer->name(), 0);
    QTimer::singleShot(200, this, &KSquaresDemoWindow::aiChooseLine);
}

void KSquaresDemoWindow::aiChooseLine()
{
    aiController ai(sGame->currentPlayerId(), sGame->lines(), sGame->squares(), sGame->boardWidth(), sGame->boardHeight());
    sGame->addLineToIndex(ai.chooseLine());
}

void KSquaresDemoWindow::gameOver(const QVector<KSquaresPlayer> & /*playerList*/)
{
    //qDebug() << "Game Over";
    QTimer::singleShot(1000, this, &KSquaresDemoWindow::gameNew);
}

