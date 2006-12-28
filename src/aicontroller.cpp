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
	
	//squaresFromLine(0);
}

int aiController::drawLine()
{
	//kDebug() << "int aiController::drawLine()" << endl;
	
	for(int i=0; i<lines.size(); i++)	//trying to get points. looking for squares with 3 lines
	{
		if(!lines.at(i))
		{
			QVector<int> adjacentSquares = squaresFromLine(i);
			for(int j=0; j<adjacentSquares.size(); j++)
			{
				
				if(countBorderLines(adjacentSquares.at(j)) == 3)	//if 3 lines, draw there to get points!
				{
					kDebug() << "AI: Drawing line at index: " << i << endl;
					return i;
				}
			}
		}
	}
	for(int i=0; i<lines.size(); i++)	//finding totally safe moves. avoiding squares with 2 lines
	{
		if(!lines.at(i))
		{
			QVector<int> adjacentSquares = squaresFromLine(i);
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
				kDebug() << "AI: Drawing line at index: " << i << endl;
				return i;
			}
		}
	}
	for(int i=0; i<lines.size(); i++)	//have to take what's left
	{
		if(!lines.at(i))
		{
			kDebug() << "AI: Drawing line at index: " << i << endl;
			return i;
		}
	}
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
	kDebug() << "AI: Square " << squareIndex << " is bordered by " << count << " lines" << endl;
	return count;
}

QVector<int> aiController::squaresFromLine(int lineIndex)
{
	//kDebug() << "Line: " << lineIndex << endl;
	QVector<int> squareList;
	if (lineDirection(lineIndex) == KS::HORIZONTAL)
	{
		squareList.append(lineIndex - ( (width+1) * (lineIndex/((width*2)+1)) ));
		squareList.append(squareList.at(0) - width);
		if(squareList.at(1) < 0)
			squareList.remove(1);
		if(squareList.at(0) >= (width*height))
			squareList.remove(0);
			
	}
	else if (lineDirection(lineIndex) == KS::VERTICAL)
	{
		squareList.append(lineIndex - ( (lineIndex/((width*2)+1))*width + (lineIndex/((width*2)+1)) + width ));
		squareList.append(squareList.at(0) - 1);
		if(lineIndex%((2*width)+1) == width)
			squareList.remove(1);
		if(lineIndex%((2*width)+1) == 2*width)
			squareList.remove(0);
	}
	//kDebug() << "Size: " << squareList.size() << endl;
	//kDebug() << "squares: " << squareList.at(0) << " " << squareList.at(1) << endl;
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
