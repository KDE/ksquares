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

#include "settings.h"

aiController::aiController(int newPlayerId, const QList<bool> &newLines, const QList<int> &newSquareOwners, int newWidth, int newHeight) : squareOwners(newSquareOwners), playerId(newPlayerId), width(newWidth), height(newHeight)
{
	linesSize = newLines.count();
	lines = new bool[linesSize];
	for (int i = 0; i < linesSize; ++i) lines[i] = newLines[i];
	srand( (unsigned)time( NULL ) );
	kDebug() << "AI: Starting AI...";
}

aiController::~aiController()
{
	delete[] lines;
}

QList<int> aiController::autoFill(int safeMovesLeft)
{
	QList<int> fillLines;
	
	// add a random safe moves while there are safe moves left
	QList<int> next;
	//kDebug() << safeMoves().isEmpty();
	while( !( (next = safeMoves()).isEmpty() ) )
	{
		int nextLine = next[rand() % next.size()];
		lines[nextLine] = true;
		//kDebug() << nextLine;
		fillLines << nextLine;
	}
	
	// safeMovesLeft times delete a line from fillLines
	for (int i = 0; i<safeMovesLeft; ++i)
	{
		if (fillLines.isEmpty()) break;
		int index = rand() % fillLines.size();
		fillLines.removeAt(index);
	}
	
	return fillLines;
}

int aiController::chooseLine() const
{
	QList<int> choiceList;
	for(int i=0; i<linesSize; i++)	//trying to get points. looking for squares with 3 lines
	{
		if(!lines[i])
		{
			QList<int> adjacentSquares = squaresFromLine(i);
			for(int j=0; j<adjacentSquares.size(); j++)
			{
				
				if(countBorderLines(adjacentSquares.at(j), lines) == 3)	//if 3 lines, draw there to get points!
				{
					choiceList.append(i);
					//kDebug() << "AI: 1. Adding" << i << "to choices";
				}
			}
		}
	}
	if(choiceList.size() != 0)
	{
		float randomFloat = ((float) rand()/(RAND_MAX + 1.0))*(choiceList.size()-1);
		int randChoice = (short)(randomFloat)/1;
		kDebug() << "AI: 1. Drawing line at index:" << choiceList.at(randChoice);
		return choiceList.at(randChoice);
	}
	
	choiceList = safeMoves();
	
	if(choiceList.size() != 0)
	{
		float randomFloat = ((float) rand()/(RAND_MAX + 1.0))*(choiceList.size()-1);
		int randChoice = (short)(randomFloat)/1;
		kDebug() << "AI: 2. Drawing line at index:" << choiceList.at(randChoice);
		return choiceList.at(randChoice);
	}
	
	choiceList.clear();
	for(int i=0; i<linesSize; i++)	//have to take what's left
	{
		if(!lines[i])
		{
			QList<int> adjacentSquares = squaresFromLine(i);
			for(int j=0; j<adjacentSquares.size(); j++)
			{
				
				if(countBorderLines(adjacentSquares.at(j), lines) == 2)	//if 2 lines (they're all that's left!)
				{
					choiceList.append(i);
					//kDebug() << "AI: 3. Adding" << i << "to choices";
				}
			}
		}
	}
	if(Settings::difficulty() == 1) //Hard(2/3)	//do some damage control :)
	{
		QList<int> goodChoiceList = chooseLeastDamaging(choiceList);
		if(goodChoiceList.size() != 0)
		{
			float randomFloat = ((float) rand()/(RAND_MAX + 1.0))*(goodChoiceList.size()-1);
			int randChoice = (short)(randomFloat)/1;
			kDebug() << "AI: 3. Drawing line at index:" << goodChoiceList.at(randChoice);
			return goodChoiceList.at(randChoice);
		}
	}
	QList<int> goodcChoiceList = chooseLeastDamaging(choiceList);
	if(choiceList.size() != 0)
	{
		float randomFloat = ((float) rand()/(RAND_MAX + 1.0))*(choiceList.size()-1);
		int randChoice = (short)(randomFloat)/1;
		kDebug() << "AI: 3. Drawing line at index:" << choiceList.at(randChoice);
		return choiceList.at(randChoice);
	}
        return 0;
}

QList<int> aiController::safeMoves() const
{
	QList<int> safeLines;
	for(int i=0; i<linesSize; i++)	//finding totally safe moves. avoiding squares with 2 lines
	{
		if(!lines[i])
		{
			QList<int> adjacentSquares = squaresFromLine(i);
			int badCount = 0;
			for(int j=0; j<adjacentSquares.size(); j++)
			{
				if(countBorderLines(adjacentSquares.at(j), lines) == 2)	//don't want to make 3 lines around a square
				{
					badCount++;
				}
			}
			if(badCount == 0)
			{
				safeLines.append(i);
				//kDebug() << "AI: 2. Adding" << i << "to choices";
			}
		}
	}
	return safeLines;
}

QList<int> aiController::chooseLeastDamaging(const QList<int> &choiceList) const
{
	//kDebug() << "AI: Checking" << choiceList.size() << "possible moves";
	QMap<int,int> linePointDamage;	//this will be a list of how damaging a certain move will be. Key = damage of move, Value = index of line
	int sidesOfSquare[4];
	bool *linesCopy = new bool[linesSize]; //make temporary local copies of lists
	for(int i=0; i<choiceList.size(); i++)	//cycle through all the possible moves
	{
		QList<int> squaresCopy = squareOwners;	//make temporary local copies of lists
		memcpy(linesCopy, lines, linesSize);	//make temporary local copies of lists
		linesCopy[choiceList.at(i)] = true;	//we're going to try drawing a line here
		
		//now it would be the next player's turn so we're going to count how many squares they would be able to get.
		int count = 0;	//this is how many points the next player will ge if you draw a line at choiceList.at(i)
		bool squareFound = false;
		do
		{
			for(int currentSquare=0; currentSquare<squaresCopy.size(); currentSquare++)	//cycle through the grid (by squares):
			{
				if(countBorderLines(sidesOfSquare, currentSquare, linesCopy) == 3)	//if we've found a square with three sides drawn:
				{
					count++;
					squareFound = true;	//we found a square so we will look again for the next
					
					for(int sideOfSquare=0; sideOfSquare<=3; sideOfSquare++)	//make the square complete in linesCopy
					{
						linesCopy[sidesOfSquare[sideOfSquare]] = true;	//draw at this squares
						
					}	//now this square is completed by the second player.
					break;	//since we found a square with 3 sides completed (now = 4), we break the 'for(currentSquare)' loop
				}
				else
				{
					squareFound = false;	//we couldn't find a square this time round so we'll try the next 'i'
				}
			}
		} while(squareFound == true);	//while we're still finding squares
		linePointDamage.insertMulti(count, choiceList.at(i));	//insert a pair with Key=count, Value=i
	}
	delete[] linesCopy;
	
	QList<int> bestMoves = linePointDamage.values(linePointDamage.begin().key());	//this is a list of the indices of the lines that are the least damaging. linePointDamage.begin() returns the 1st pair in the QMap, sorted in ascending order by Key (damage of move)
	return bestMoves;
}

int aiController::countBorderLines(int *sidesOfSquare, int squareIndex, const bool *linesList) const
{
	int count = 0;
	
	linesFromSquare(sidesOfSquare, squareIndex);
	
	//TODO: replace this with a QList 'count' type function?
	if(linesList[sidesOfSquare[0]] == true)
		count++;
	if(linesList[sidesOfSquare[1]] == true)
		count++;
	if(linesList[sidesOfSquare[2]] == true)
		count++;
	if(linesList[sidesOfSquare[3]] == true)
		count++;
	//kDebug() << "AI: Square" << squareIndex << "is bordered by" << count << "lines";
	return count;
}

int aiController::countBorderLines(int squareIndex, const bool *linesList) const
{
	int tempLineList[4];
	return countBorderLines(tempLineList, squareIndex, linesList);
}

QList<int> aiController::squaresFromLine(int lineIndex) const
{
	//kDebug() << "Line:" << lineIndex;
	QList<int> squareList;
	if (lineDirection(lineIndex) == KSquares::HORIZONTAL)
	{
		squareList.append(lineIndex - ( (width+1) * (lineIndex/((width*2)+1)) ));
		squareList.append(squareList.at(0) - width);
		if(squareList.at(1) < 0)
			squareList.removeAt(1);
		if(squareList.at(0) >= (width*height))
			squareList.removeAt(0);
			
	}
	else if (lineDirection(lineIndex) == KSquares::VERTICAL)
	{
		squareList.append(lineIndex - ( (lineIndex/((width*2)+1))*width + (lineIndex/((width*2)+1)) + width ));
		squareList.append(squareList.at(0) - 1);
		if(lineIndex%((2*width)+1) == width)
			squareList.removeAt(1);
		if(lineIndex%((2*width)+1) == 2*width)
			squareList.removeAt(0);
	}
	//kDebug() << "Size:" << squareList.size();
	//kDebug() << "squares:" << squareList.at(0) << " " << squareList.at(1);
	return squareList;
}

void aiController::linesFromSquare(int *linesFromSquare, int squareIndex) const
{
	int index1 = (squareIndex/width) * ((2*width) + 1) + (squareIndex%width);
	int index2 = index1 + width;
	int index3 = index2 + 1;
	int index4 = index3 + width;
	linesFromSquare[0] = index1;
	linesFromSquare[1] = index2;
	linesFromSquare[2] = index3;
	linesFromSquare[3] = index4;
}

KSquares::Direction aiController::lineDirection(int lineIndex) const
{
	int index2 = lineIndex % ((2*width) + 1);
	KSquares::Direction dir;
	if(index2 < width)
		dir = KSquares::HORIZONTAL;
	else
		dir = KSquares::VERTICAL;
	
	return dir;
}
