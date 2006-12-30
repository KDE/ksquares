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
		KSquaresPlayer(QString newName = "Player", bool isPlayerHuman = true) {setName(newName); human = isPlayerHuman; playerScore = 0;}	//defaults to human player
		KSquaresPlayer(const KSquaresPlayer& ksp) {human = ksp.isHuman(); playerScore = ksp.score(); setName(ksp.name());}	//copy constructor
		
		void setName(QString newName) {playerName = newName;}
		QString name() const {return playerName;}
		
		void setScore(int newScore) {playerScore = newScore;}
		int score() const {return playerScore;}
		void incScore() {playerScore++;}
		
		bool isHuman() const {return human;}
		
		bool operator<(const KSquaresPlayer& player) const {return score() < player.score();}
	
	private:
		bool human;	//is the player a human?
		int playerScore;
		QString playerName;
};

#endif // KSQUARESPLAYER_H
