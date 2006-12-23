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

class KSquaresPlayer
{
	public:
		KSquaresPlayer(bool isPlayerHuman = true) {human = isPlayerHuman; playerScore = 0;}	//defaults to human player
		KSquaresPlayer(const KSquaresPlayer& ksp) {human = ksp.isHuman(); playerScore = ksp.score();}	//copy constructor
		void setScore(int newScore) {playerScore = newScore;}
		int score() const {return playerScore;}
		void incScore() {playerScore++;}
		bool isHuman() const {return human;}
	
	private:
		bool human;	//is the player a human?
		int playerScore;
};

#endif // KSQUARESPLAYER_H
