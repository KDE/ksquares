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

aiController::aiController(int newPlayerId, QList<bool> newLines, QList<int> newSquareOwners, int newWidth, int newHeight) : squareOwners(newSquareOwners), lines(newLines), playerId(newPlayerId), width(newWidth), height(newHeight)
{
	kDebug() << "AI: Starting AI..." << endl;
	
	//squaresFromLine(0);
}

int aiController::chooseLine()
{
	QList<int> choiceList;
	for(int i=0; i<lines.size(); i++)	//trying to get points. looking for squares with 3 lines
	{
		if(!lines.at(i))
		{
			QList<int> adjacentSquares = squaresFromLine(i);
			for(int j=0; j<adjacentSquares.size(); j++)
			{
				
				if(countBorderLines(adjacentSquares.at(j)) == 3)	//if 3 lines, draw there to get points!
				{
					choiceList.append(i);
					//kDebug() << "AI: 1. Adding " << i << " to choices" << endl;
				}
			}
		}
	}
	if(choiceList.size() != 0)
	{
		srand( (unsigned)time( NULL ) );
		float randomFloat = ((float) rand()/(RAND_MAX + 1.0))*(choiceList.size()-1);
		int randChoice = (short)(randomFloat)/1;
		kDebug() << "AI: 1. Drawing line at index: " << choiceList.at(randChoice) << endl;
		return choiceList.at(randChoice);
	}
	
	choiceList.clear();
	for(int i=0; i<lines.size(); i++)	//finding totally safe moves. avoiding squares with 2 lines
	{
		if(!lines.at(i))
		{
			QList<int> adjacentSquares = squaresFromLine(i);
			int badCount = 0;
			for(int j=0; j<adjacentSquares.size(); j++)
			{
				
				if(countBorderLines(adjacentSquares.at(j)) == 2)	//don't want to make 3 lines around a square
				{
					badCount++;
				}
			}
			if(badCount == 0)
			{
				choiceList.append(i);
				//kDebug() << "AI: 2. Adding " << i << " to choices" << endl;
			}
		}
	}
	if(choiceList.size() != 0)
	{
		srand( (unsigned)time( NULL ) );
		float randomFloat = ((float) rand()/(RAND_MAX + 1.0))*(choiceList.size()-1);
		int randChoice = (short)(randomFloat)/1;
		kDebug() << "AI: 2. Drawing line at index: " << choiceList.at(randChoice) << endl;
		return choiceList.at(randChoice);
	}
	
	choiceList.clear();
	for(int i=0; i<lines.size(); i++)	//have to take what's left
	{
		if(!lines.at(i))
		{
			choiceList.append(i);
			//kDebug() << "AI: 3. Adding " << i << " to choices" << endl;
		}
	}
	if(choiceList.size() != 0)
	{
		srand( (unsigned)time( NULL ) );
		float randomFloat = ((float) rand()/(RAND_MAX + 1.0))*(choiceList.size()-1);
		int randChoice = (short)(randomFloat)/1;
		kDebug() << "AI: 3. Drawing line at index: " << choiceList.at(randChoice) << endl;
		return choiceList.at(randChoice);
	}
}

int aiController::countBorderLines(int squareIndex)
{
	int count = 0;
	
	QList<int> lineList = linesFromSquare(squareIndex);
	
	if(lines.at(lineList.at(0)) == true)
		count++;
	if(lines.at(lineList.at(1)) == true)
		count++;
	if(lines.at(lineList.at(2)) == true)
		count++;
	if(lines.at(lineList.at(3)) == true)
		count++;
	//kDebug() << "AI: Square " << squareIndex << " is bordered by " << count << " lines" << endl;
	return count;
}

QList<int> aiController::squaresFromLine(int lineIndex)
{
	//kDebug() << "Line: " << lineIndex << endl;
	QList<int> squareList;
	if (lineDirection(lineIndex) == KS::HORIZONTAL)
	{
		squareList.append(lineIndex - ( (width+1) * (lineIndex/((width*2)+1)) ));
		squareList.append(squareList.at(0) - width);
		if(squareList.at(1) < 0)
			squareList.removeAt(1);
		if(squareList.at(0) >= (width*height))
			squareList.removeAt(0);
			
	}
	else if (lineDirection(lineIndex) == KS::VERTICAL)
	{
		squareList.append(lineIndex - ( (lineIndex/((width*2)+1))*width + (lineIndex/((width*2)+1)) + width ));
		squareList.append(squareList.at(0) - 1);
		if(lineIndex%((2*width)+1) == width)
			squareList.removeAt(1);
		if(lineIndex%((2*width)+1) == 2*width)
			squareList.removeAt(0);
	}
	//kDebug() << "Size: " << squareList.size() << endl;
	//kDebug() << "squares: " << squareList.at(0) << " " << squareList.at(1) << endl;
	return squareList;
}

QList<int> aiController::linesFromSquare(int squareIndex)
{
	QList<int> lineList;
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
