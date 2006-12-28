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

aiController::aiController(int newPlayerId, QVector<bool> newLines, QVector<int> newSquareOwners, int newWidth, int newHeight) : squareOwners(newSquareOwners), lines(newLines), playerId(newPlayerId), width(newWidth), height(newHeight)
{
	kDebug() << "AI: Starting AI..." << endl;
}

int aiController::drawLine()
{
	int indexChoice;
	srand( (unsigned)time( NULL ) );
	//kDebug() << "int aiController::drawLine()" << endl;
	do
	{
		float randomFloat = ((float) rand()/(RAND_MAX + 1.0))*(lines.size()-1);
		indexChoice = (short)(randomFloat)/1;
	}while(lines.at(indexChoice));	//continue while 'that' square is taken
		
	kDebug() << "AI: Drawing line at index: " << indexChoice << endl;
	return indexChoice;
}

int aiController::countBorderLines(int squareIndex)
{
	int count = 0;
	
	QVector<int> lineList = linesFromSquare(squareIndex);
	
	if(lines.at(lineList.at(0)) == true)
		count++;
	if(lines.at(lineList.at(1)) == true)
		count++;
	if(lines.at(lineList.at(2)) == true)
		count++;
	if(lines.at(lineList.at(3)) == true)
		count++;
	
	return count;
}

QVector<int> aiController::squaresFromLine(int lineIndex)
{
	QVector<int> squareList;
	if (lineDirection(lineIndex) == KS::HORIZONTAL)
	{
		squareList.append(lineIndex - ( (width+1) * (lineIndex/((width*2)+1)) ));
		squareList.append(squareList.at(0) - width);
		if(squareList.at(0) < 0)
			squareList.remove(0);
		if(squareList.at(1) >= (width*height))
			squareList.remove(1);
			
	}
	else if (lineDirection(lineIndex) == KS::VERTICAL)	//TODO: fix wrap-around
	{
		squareList.append(lineIndex - ( (lineIndex/((width*2)+1))*width + (lineIndex/((width*2)+1)) + width ));
		squareList.append(squareList.at(0) - 1);
	}
	return squareList;
}

QVector<int> aiController::linesFromSquare(int squareIndex)
{
	QVector<int> lineList;
	int index1 = (squareIndex/width) * ((2*width) + 1) + (squareIndex%width);
	int index2 = index1 + width;
	int index3 = index2 + 1;
	int index4 = index3 + width;
	lineList.append(index1);
	lineList.append(index2);
	lineList.append(index3);
	lineList.append(index4);
	return lineList;
}

KS::Direction aiController::lineDirection(int lineIndex)
{
	int index2 = lineIndex % ((2*width) + 1);
	KS::Direction dir;
	if(index2 < width)
		dir = KS::HORIZONTAL;
	else
		dir = KS::VERTICAL;
	
	return dir;
}

#include "aicontroller.moc"
