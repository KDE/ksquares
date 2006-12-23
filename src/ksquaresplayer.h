/***************************************************************************
 *   Copyright (C) 2006 by Matthew Williams    <matt@milliams.com>         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef KSQUARESPLAYER_H
#define KSQUARESPLAYER_H

#include <QString>

class KSquaresPlayer
{
	public:
		KSquaresPlayer(QString newName = "Player", bool isPlayerHuman = true) {human = isPlayerHuman; playerScore = 0; playerName = newName;}	//defaults to human player
		//KSquaresPlayer(QString newName, bool isPlayerHuman = true) {human = isPlayerHuman; playerScore = 0; playerName = newName;}
		KSquaresPlayer(const KSquaresPlayer& ksp) {human = ksp.isHuman(); playerScore = ksp.score();}	//copy constructor
		void setName(QString newName) {playerName = newName;}
		QString name() {return playerName;}
		void setScore(int newScore) {playerScore = newScore;}
		int score() const {return playerScore;}
		void incScore() {playerScore++;}
		bool isHuman() const {return human;}
	
	private:
		bool human;	//is the player a human?
		int playerScore;
		QString playerName;
};

#endif // KSQUARESPLAYER_H
