/***************************************************************************
 *   Copyright (C) 2006 by Matthew Williams    <matt@milliams.com>         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef AICONTROLLER_H
#define AICONTROLLER_H

#include <QObject>
#include <QVector>

namespace KS
{
	enum Direction {HORIZONTAL, VERTICAL};
}

/**
 * @short AI Controller for KSquares
 *
 * When playing a game of squares there are a number of stages the game goes through:
 * @li The random line placement stage. Where players are just placing lines while trying to not complete the third side of any squares
 * @li Next players will try to only draw the third side of a square if it will only give the opponent the minimum amount of points
 * @li Finally, the more advanced player will, at the end of a large run of squares leave a small area at the end, forcing the opponent to take only that small section, leaving another large area open to him.
 *
 * @todo Write proper AI
 * 
 * @author Matt Williams <matt@milliams.com>
 */

class aiController : public QObject
{
	Q_OBJECT
	
	public:
		aiController(int newPlayerId, QVector<bool> newLines, QVector<int> newSquareOwners, int newWidth, int newHeight);
		/**
		 * Choses where to draw the line
		 * Currently only choses randomly :S
		 *
		 * @return The index of the line from "QVector<bool> lines"
		 */
		int drawLine();
		
	public slots:

	protected:
		/**
		 * @param squareIndex the index of the square (relates to @ref squareOwners ) 
		 *
		 * @return The number of lines currently drawn around a specific square
		 */
		int countBorderLines(int squareIndex);
		QVector<int> squaresFromLine(int lineIndex);
		QVector<int> linesFromSquare(int squareIndex);
		KS::Direction lineDirection(int lineIndex);
		
		QVector<int> squareOwners;
		QVector<bool> lines;
		int playerId;
		int width;
		int height;
		
	signals:
		
};

#endif // KSQUARES_H
