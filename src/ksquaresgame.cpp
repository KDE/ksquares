/*
    SPDX-FileCopyrightText: 2006 Matthew Williams <matt@milliams.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "ksquaresgame.h"

#include <QDebug>

//generated
#include "settings.h"

KSquaresGame::KSquaresGame()
{
    //qDebug() << "Constructing Game";
    gameInProgress = false;
}

KSquaresGame::~KSquaresGame()
{
    //qDebug() << "Destroying game";
    gameInProgress = false;
}

void KSquaresGame::createGame(const QVector<KSquaresPlayer> &startPlayers, int startWidth, int startHeight)
{
    resetEverything();  //reset everything
    //qDebug() << "Creating Game with" << startPlayers.size() << "player(s)";

    //BEGIN Initialisation
    width = startWidth;
    height = startHeight;
    for (int i = 0; i < startPlayers.size(); i++) {
        players.append(startPlayers[i]);
    }
    for (int i = 0; i < (2 * width * height + width + height); i++) {
        lineList.append(false);
    }
    for (int i = 0; i < (width * height); i++) {
        squareOwnerTable.append(-1);
    }
    //END Initialisation

    //qDebug() << "Game Starting";
    nextPlayer();
}

void KSquaresGame::switchPlayer()
{
    anotherGo = false;
    currentPlayerId() >= (players.size() - 1) ? i_currentPlayerId = 0 : i_currentPlayerId++;
}

int KSquaresGame::nextPlayer()
{
    anotherGo = false;  //just to reset the variable
    currentPlayerId() >= (players.size() - 1) ? i_currentPlayerId = 0 : i_currentPlayerId++;
    //qDebug()<< "- Moving to next player:" << currentPlayer()->name() << "(" << currentPlayerId() << ")";
    //qDebug() << "-";
    Q_EMIT takeTurnSig(currentPlayer());

    return currentPlayerId();
}

void KSquaresGame::playerSquareComplete(int index)
{
    //qDebug() << "- - " << currentPlayer()->name() << "(" << currentPlayerId() << ") has completed a square";
    anotherGo = true;

    squareOwnerTable[index] = currentPlayerId();    //add square to index
    Q_EMIT drawSquare(index, currentPlayer()->colour());
    currentPlayer()->incScore();

    int totalPoints = 0;
    for (int i = 0; i < players.size(); i++) {
        totalPoints += players.at(i).score();
    }
    //qDebug() << "- - Square Completed";
    if (totalPoints >= width * height) { //if the board is full
        //qDebug() << "Game Over";
        gameInProgress = false;
        Q_EMIT gameOver(players);
    }
}

void KSquaresGame::tryEndGo()
{
    //qDebug() << "- - Trying to end go";
    if (anotherGo) {
        if (gameInProgress) {
            //qDebug() << "- - - Having another go";
            //qDebug() << "-";
            anotherGo = false;
            Q_EMIT takeTurnSig(currentPlayer());
        }
    } else {
        //qDebug() << "- - - Go ending";
        if (!currentPlayer()->isHuman()) {
            Q_EMIT highlightMove(lastLine);
        }
        nextPlayer();
    }
}

void KSquaresGame::resetEverything()
{
    //qDebug() << "Game Values Resetting";
    numOfPlayers = 0;
    players.resize(0);
    lineList.clear();
    squareOwnerTable.clear();
    width = 0;
    height = 0;
    i_currentPlayerId = -1;
    anotherGo = false;
    gameInProgress = false;
    lastLine = -1;
}

void KSquaresGame::addLineToIndex(int index)
{
    if (lineList[index] == true) {  //if there is already a line
        qWarning() << "KSquaresGame::addLineToIndex():"
                   << "trying to add line already there!";
        return;
    }
    lineList[index] = true;
    lastLine = index;

    Q_EMIT drawLine(index, Settings::lineColor());

    if (gameInProgress) {
        checkForNewSquares();
    }
}

void KSquaresGame::checkForNewSquares()
{
    for (int i = 0; i < (width * height); i++) { //cycle through every box..
        if (squareOwnerTable.at(i) == -1) { //..checking it if there is no current owner
            //indices of the lines surrounding the box; Correlates to "QVector<bool> lineList"
            int index1 = (i / width) * ((2 * width) + 1) + (i % width);
            int index2 = index1 + width;
            int index3 = index2 + 1;
            int index4 = index3 + width;
            //cout << index1 << "," << index2 << "," << index3 << "," << index4 << " - " << lineList.size() << endl;
            if (lineList.at(index1) && lineList.at(index2) && lineList.at(index3) && lineList.at(index4)) {
                //qDebug() << "- - Square" << i << "completed.";
                playerSquareComplete(i);
            }
        }
    }
    //Q_EMIT lineDrawnSig();
    tryEndGo();
}

