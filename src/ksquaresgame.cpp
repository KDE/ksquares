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

#include <kdebug.h>

#include "ksquaresgame.h"

KSquaresGame::KSquaresGame()
{
	kdDebug() << "Constructing Game" << endl;
}

KSquaresGame::~KSquaresGame()
{
	kdDebug() << "Destroying game" << endl;
}

/*void KSquaresGame::createGame(uint players, int startWidth, int startHeight)
{
	kdDebug() << "Creating Game with " << players << " player(s)" << endl;
	width = startWidth;
	height = startHeight;
	currentPlayerId = 0;
	points.resize(0);
	points.fill(0, players);
}*/

void KSquaresGame::createGame(QVector<KSquaresPlayer> startPlayers, int startWidth, int startHeight)
{
	kdDebug() << "Creating Game with " << startPlayers.size() << " player(s)" << endl;
	width = startWidth;
	height = startHeight;
	currentPlayerId = -1;
	for(int i=0; i < startPlayers.size(); i++)	//???
	{
		players.append(startPlayers[i]);	//???
	}
	//points.resize(0);
	//points.fill(0, players.size());
}

void KSquaresGame::startGame()
{
	kdDebug() << "Game Starting" << endl;
	nextPlayer();
}

int KSquaresGame::nextPlayer()
{
	anotherGo = false;	//just ro reset the variable
	currentPlayerId >= (players.size()-1) ? currentPlayerId = 0 : currentPlayerId++;
	kdDebug() << "- Moving to next player: player " << currentPlayer() << endl;
	emit playerChangedSig(currentPlayer());
	
	return currentPlayer();
}

void KSquaresGame::playerSquareComplete(int index)
{
	kdDebug() << "- - Player " << currentPlayer() << " has completed a square" << endl;
	anotherGo = true;
	emit setSquareOwnerSig(index, currentPlayer());
	//points[currentPlayer()-1]++;
	players[currentPlayer()].incScore();
	int totalPoints=0;
	for (int i=0; i < players.size(); i++)
	{
		totalPoints += players.at(i).score();
	}
	//kdDebug() << "total points: " << totalPoints << endl;
	//kdDebug() << "width*height: " << width*height << endl;
	if (totalPoints >= width*height)	//if the board is full
	{
		kdDebug() << "Game Over" << endl;
		emit gameOverSig(players);
		endGame();
	}
}

void KSquaresGame::tryEndGo()
{
	kdDebug() << "- - Line placed, trying to end go" << endl;
	if (not anotherGo)
	{
		kdDebug() << "- - - Go ending" << endl;
		nextPlayer();
		return;
	}
	else
	{
		kdDebug() << "- - - Having another go" << endl;
		anotherGo = false;
	}
}

void KSquaresGame::endGame()
{
	kdDebug() << "Game Values Resetting" << endl;
	numOfPlayers = 0;
	width = 0;
	height = 0;
	currentPlayerId = -1;
	//points.resize(0);
	anotherGo = false;
}

#include "ksquaresgame.moc"
