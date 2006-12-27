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

#include <QtCore>

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
		aiController(int newPlayerId);
		void setLines(QVector<bool> newLines);
		void setSquareOwners(QVector<int> newSquareOwners);
		/**
		 * Choses where to draw the line
		 * Currently only choses randomly :S
		 *
		 * @return The index of the line from "QVector<bool> lines"
		 */
		int drawLine();
		
	public slots:

	protected:
		QVector<int> squareOwners;
		QVector<bool> lines;
		int playerId;
		
	signals:
		
};

#endif // KSQUARES_H
