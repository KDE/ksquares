/***************************************************************************
 *   Copyright (C) 2006 by Matthew Williams    <matt@milliams.com>         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include <kdebug.h>

#include "ksquaresgame.h"

KSquaresGame::KSquaresGame()
{
	kDebug() << "Constructing Game" << endl;
	gameInProgress = false;
}

KSquaresGame::~KSquaresGame()
{
	kDebug() << "Destroying game" << endl;
	gameInProgress = false;
}

void KSquaresGame::createGame(QVector<KSquaresPlayer> startPlayers, int startWidth, int startHeight)
{
	endGame();	//reset everything
	kDebug() << "Creating Game with " << startPlayers.size() << " player(s)" << endl;
	width = startWidth;
	height = startHeight;
	i_currentPlayerId = -1;
	for(int i=0; i < startPlayers.size(); i++)	//???
	{
		players.append(startPlayers[i]);	//???
	}
	gameInProgress = false;
	//points.resize(0);
	//points.fill(0, players.size());
}

void KSquaresGame::startGame()
{
	kDebug() << "Game Starting" << endl;
	gameInProgress = true;
	nextPlayer();
}

int KSquaresGame::nextPlayer()
{
	anotherGo = false;	//just to reset the variable
	currentPlayerId() >= (players.size()-1) ? i_currentPlayerId = 0 : i_currentPlayerId++;
	kDebug() << endl << "- Moving to next player: " << currentPlayer()->name() << "(" << currentPlayerId() << ")" << endl;
	kDebug() << "-" << endl;
	emit takeTurnSig(currentPlayer());
	
	return currentPlayerId();
}

void KSquaresGame::playerSquareComplete(int index)
{
	kDebug() << "- - " << currentPlayer()->name() << "(" << currentPlayerId() << ") has completed a square" << endl;
	anotherGo = true;
	emit setSquareOwnerSig(index, currentPlayerId());
	//points[currentPlayer()-1]++;
	currentPlayer()->incScore();
	int totalPoints=0;
	for (int i=0; i < players.size(); i++)
	{
		totalPoints += players.at(i).score();
	}
	//kDebug() << "total points: " << totalPoints << endl;
	//kDebug() << "width*height: " << width*height << endl;
	kDebug() << "- - Square Completed" << endl;
	//kDebug() << "- - totalPoints: " << totalPoints << " (/" << width*height << ")" << endl;
	if (totalPoints >= width*height)	//if the board is full
	{
		kDebug() << "Game Over" << endl;
		gameInProgress = false;
		emit gameOverSig(players);
		//endGame();
	}
}

void KSquaresGame::tryEndGo()
{
	kDebug() << "- - Trying to end go" << endl;
	if (anotherGo)
	{
		if(gameInProgress)
		{
			kDebug() << "- - - Having another go" << endl;
			kDebug() << "-" << endl;
			anotherGo = false;
			emit takeTurnSig(currentPlayer());
		}
	}
	else
	{
		kDebug() << "- - - Go ending" << endl;
		nextPlayer();
	}
}

void KSquaresGame::endGame()
{
	kDebug() << "Game Values Resetting" << endl;
	numOfPlayers = 0;
	players.resize(0);
	width = 0;
	height = 0;
	i_currentPlayerId = -1;
	anotherGo = false;
	gameInProgress = false;
}

#include "ksquaresgame.moc"
