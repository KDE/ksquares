/***************************************************************************
 *   Copyright (C) 2006 by Matthew Williams    <matt@milliams.com>         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "ksquaresgame.h"

#include <kdebug.h>

//generated
#include "settings.h"

KSquaresGame::KSquaresGame()
{
	kDebug() << "Constructing Game";
	gameInProgress = false;
}

KSquaresGame::~KSquaresGame()
{
	kDebug() << "Destroying game";
	gameInProgress = false;
}

void KSquaresGame::createGame(const QVector<KSquaresPlayer> &startPlayers, int startWidth, int startHeight)
{
	resetEverything();	//reset everything
	kDebug() << "Creating Game with" << startPlayers.size() << "player(s)";
	
	//BEGIN Initialisation
	width = startWidth;
	height = startHeight;
	for(int i=0; i < startPlayers.size(); i++)
	{
		players.append(startPlayers[i]);
	}
	for(int i = 0; i < (2*width*height + width + height); i++)
	{
		lineList.append(false);
	}
	for(int i = 0; i < (width*height); i++)
	{
		squareOwnerTable.append(-1);
	}
	//END Initialisation
	
	kDebug() << "Game Starting";
	nextPlayer();
}

int KSquaresGame::nextPlayer()
{
	anotherGo = false;	//just to reset the variable
	currentPlayerId() >= (players.size()-1) ? i_currentPlayerId = 0 : i_currentPlayerId++;
	kDebug()<< "- Moving to next player:" << currentPlayer()->name() << "(" << currentPlayerId() << ")";
	kDebug() << "-";
	emit takeTurnSig(currentPlayer());
	
	return currentPlayerId();
}

void KSquaresGame::playerSquareComplete(int index)
{
	kDebug() << "- - " << currentPlayer()->name() << "(" << currentPlayerId() << ") has completed a square";
	anotherGo = true;
	
	squareOwnerTable[index] = currentPlayerId();	//add square to index
	emit drawSquare(index, currentPlayer()->colour());
	currentPlayer()->incScore();
	
	int totalPoints=0;
	for (int i=0; i < players.size(); i++)
	{
		totalPoints += players.at(i).score();
	}
	kDebug() << "- - Square Completed";
	if (totalPoints >= width*height)	//if the board is full
	{
		kDebug() << "Game Over";
		gameInProgress = false;
		emit gameOver(players);
	}
}

void KSquaresGame::tryEndGo()
{
	kDebug() << "- - Trying to end go";
	if (anotherGo)
	{
		if(gameInProgress)
		{
			kDebug() << "- - - Having another go";
			kDebug() << "-";
			anotherGo = false;
			emit takeTurnSig(currentPlayer());
		}
	}
	else
	{
		kDebug() << "- - - Go ending";
		if (!currentPlayer()->isHuman())
		{
			emit highlightMove(lastLine);
		}
		nextPlayer();
	}
}

void KSquaresGame::resetEverything()
{
	kDebug() << "Game Values Resetting";
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
	if (lineList[index] == true)	//if there is already a line
	{
		kWarning() << "KSquaresGame::addLineToIndex():" 
				   << "trying to add line already there!";
		return;
	}
	lineList[index] = true;
	lastLine = index;
	
	emit drawLine(index, Settings::lineColor());
	
	if (gameInProgress)
		checkForNewSquares();
}

void KSquaresGame::checkForNewSquares()
{
	for(int i=0; i < (width*height); i++)	//cycle through every box..
	{
		if (squareOwnerTable.at(i) == -1)	//..checking it if there is no current owner
		{
			//indices of the lines surrounding the box; Correlates to "QVector<bool> lineList"
			int index1 = (i/width) * ((2*width) + 1) + (i%width);
			int index2 = index1 + width;
			int index3 = index2 + 1;
			int index4 = index3 + width;
			//cout << index1 << "," << index2 << "," << index3 << "," << index4 << " - " << lineList.size() << endl;
			if (lineList.at(index1) && lineList.at(index2) && lineList.at(index3) && lineList.at(index4))
			{
				kDebug() << "- - Square" << i << "completed.";
				playerSquareComplete(i);
			}
		}
	}
	//emit lineDrawnSig();
	tryEndGo();
}

#include "ksquaresgame.moc"
