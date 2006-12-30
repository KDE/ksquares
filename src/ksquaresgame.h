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

#include "ksquaresplayer.h"

class KSquaresGame : public QObject
{
	Q_OBJECT
	
	public:
		KSquaresGame();
		~KSquaresGame();
		
		void createGame(uint players, int startWidth, int startHeight);
		void createGame(QVector<KSquaresPlayer> startPlayers, int startWidth, int startHeight);
		
		int currentPlayerId() const {return i_currentPlayerId;}
		KSquaresPlayer* currentPlayer() {return &players[currentPlayerId()];}
		
		int nextPlayer();	// moves play control to the next player, looping round when necessary 
		
	public slots:
		void startGame();
		void playerSquareComplete(int index);
		void tryEndGo();
		void endGame();

	protected:
		int numOfPlayers;
		int i_currentPlayerId;	// from 0 to (n-1); -1 is null player
		QVector<int> points;
		bool anotherGo;
		int width;
		int height;
		QVector<KSquaresPlayer> players;
		bool gameInProgress;
		
	signals:
		void setSquareOwnerSig(int,int);
		void takeTurnSig(KSquaresPlayer*);	//emit the new curent player
		void gameOverSig(QVector<KSquaresPlayer>);
};

#endif // KSQUARESGAME_H
