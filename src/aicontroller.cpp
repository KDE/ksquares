/***************************************************************************
 *   Copyright (C) 2006 by Matthew Williams    <matt@milliams.com>         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "aicontroller.h"

#include <ctime>
#include <kdebug.h>

aiController::aiController(int newPlayerId)
{
	kDebug() << "AI: Starting AI..." << endl;
	playerId = newPlayerId;
}

void aiController::setLines(QVector<bool> newLines)
{
	kDebug() << "AI: Reading lines" << endl;
	lines = newLines;
	//kdDebug() << "AI: - lines.size(): " << lines.size() << endl;
}

void aiController::setSquareOwners(QVector<int> newSquareOwners)
{
	kDebug() << "AI: Reading square ownerships" << endl;
	squareOwners = newSquareOwners;
	//kdDebug() << "AI: - newSquareOwners.size(): " << newSquareOwners.size() << endl;
}

int aiController::drawLine()
{
	int indexChoice;
	srand( (unsigned)time( NULL ) );
	do
	{
		float randomFloat = ((float) rand()/(RAND_MAX + 1.0))*(lines.size()-1);
		indexChoice = (short)(randomFloat)/1;
	}while(lines.at(indexChoice));	//continue while 'that' square is taken
		
	kDebug() << "AI: Drawing line at index: " << indexChoice << endl;
	return indexChoice;
}

#include "aicontroller.moc"
