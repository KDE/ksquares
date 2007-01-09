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

/**
 * @short Player class for KSquares
 *
 * Class containg All the information about a player.
 * 
 * @author Matt Williams <matt@milliams.com>
 */

class KSquaresPlayer
{
	public:
		/**
		 * Creates a player
		 * 
		 * @param newName The displayable name of the player
		 * @param isPlayerHuman is the player human (or AI)
		 */
		KSquaresPlayer(QString newName = "Player", bool isPlayerHuman = true) {setName(newName); human = isPlayerHuman; playerScore = 0;}	//defaults to human player
		/**
		 * Copy constructor
		 * 
		 * @param ksp the KSquaresPlayer you want to copy
		 */
		KSquaresPlayer(const KSquaresPlayer& ksp) {human = ksp.isHuman(); playerScore = ksp.score(); setName(ksp.name());}	//copy constructor
		
		/**
		 * Sets the players name
		 * 
		 * @param newName The displayable name of the player
		 */
		void setName(QString newName) {playerName = newName;}
		/**
		 * Sets the players name
		 * 
		 * @return The displayable name of the player
		 */
		QString name() const {return playerName;}
		
		/**
		 * Sets the players score
		 * 
		 * @param newScore The players new score
		 */
		void setScore(int newScore) {playerScore = newScore;}
		/**
		 * @return The players current score
		 */
		int score() const {return playerScore;}
		///Increase the players score by 1
		void incScore() {playerScore++;}
		
		/**
		 * @return Is the player human
		 */
		bool isHuman() const {return human;}
		
		/**
		 * Used in sort algorithm. Compares the player's score (not their height or age or anything :D)
		 */
		bool operator<(const KSquaresPlayer& player) const {return score() < player.score();}
	
	protected:
		/// Is the player a human
		bool human;
		/// Player's current score
		int playerScore;
		/// The player's displayable name
		QString playerName;
};

#endif // KSQUARESPLAYER_H
