/***************************************************************************
 *   Copyright (C) 2006 by Matthew Williams    <matt@milliams.com>         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef KSQUARESGAME_H
#define KSQUARESGAME_H

#include <QObject>
#include <QVector>

#include "ksquaresplayer.h"

class QColor;

/**
 * @short The game controller
 *
 * Keeps charge of the game. Everything you'd expect really.
 * 
 * @author Matt Williams <matt@milliams.com>
 */

class KSquaresGame : public QObject
{
	Q_OBJECT
	
	public:
		KSquaresGame();
		~KSquaresGame();
		
		void createGame(QVector<KSquaresPlayer> startPlayers, int startWidth, int startHeight);
		
		int currentPlayerId() const {return i_currentPlayerId;}
		KSquaresPlayer* currentPlayer() {return &players[currentPlayerId()];}
		
		QList<int> squares() {return squareOwnerTable;}
		QList<bool> lines() {return lineList;}
		int boardWidth() const {return width;}	//should be in game class
		int boardHeight() const {return height;}	//should be in game class
		
	public slots:
		void playerSquareComplete(int index);
		void tryEndGo();
		void addLineToIndex(int index);

	protected:
		int nextPlayer();	// moves play control to the next player, looping round when necessary
		void resetEverything();
		
		void checkForNewSquares();
		
		// Static throughout each game
		int numOfPlayers;
		int width;
		int height;
		QList<int> squareOwnerTable;	// Along top row, then 2nd row et cetera. Size = width*height
		QList<bool> lineList;	// In this order: top row of horizontal lines, first row of vertical lines, 2nd row of horizontal lines etc... Size: 2*width*height + width + height
		
		// Updated as the game progresses
		QVector<KSquaresPlayer> players;
		
		// Probably changes every go
		int i_currentPlayerId;	// from 0 to (n-1); -1 is null player
		bool anotherGo;
		bool gameInProgress;
		
	signals:
		//void setSquareOwnerSig(int,int);
		void takeTurnSig(KSquaresPlayer*);	//emit the new curent player
		void gameOverSig(QVector<KSquaresPlayer>);	//for scoreboard purposes
		void drawLine(int,QColor);	//int == lineList index
		void drawSquare(int,QColor);	//int == squareOwnerTable index
};

#endif // KSQUARESGAME_H
