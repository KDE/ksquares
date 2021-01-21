/*
    SPDX-FileCopyrightText: 2006 Matthew Williams <matt@milliams.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "aicontroller.h"

#include <ctime>
#include <QDebug>

#include <QSet>
#include <QRandomGenerator>

#include "settings.h"

aiController::aiController(int newPlayerId, const QList<bool> &newLines, const QList<int> &newSquareOwners, int newWidth, int newHeight) : squareOwners(newSquareOwners), playerId(newPlayerId), width(newWidth), height(newHeight)
{
    linesSize = newLines.count();
    lines = new bool[linesSize];
    for (int i = 0; i < linesSize; ++i) {
        lines[i] = newLines[i];
    }
    //qDebug() << "AI: Starting AI level" << Settings::difficulty();
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
    ////qDebug() << safeMoves().isEmpty();
    while (!((next = safeMoves()).isEmpty())) {
        int nextLine = next[rand() % next.size()];
        lines[nextLine] = true;
        ////qDebug() << nextLine;
        fillLines << nextLine;
    }

    // safeMovesLeft times delete a line from fillLines
    for (int i = 0; i < safeMovesLeft; ++i) {
        if (fillLines.isEmpty()) {
            break;
        }
        int index = QRandomGenerator::global()->bounded(fillLines.size());
        fillLines.removeAt(index);
    }

    return fillLines;
}

int aiController::chooseLine() const
{
    QList<int> choiceList;
    for (int i = 0; i < linesSize; i++) { //trying to get points. looking for squares with 3 lines
        if (!lines[i]) {
            QList<int> adjacentSquares = squaresFromLine(i);
            for (int j = 0; j < adjacentSquares.size(); j++) {

                if (countBorderLines(adjacentSquares.at(j), lines) == 3) { //if 3 lines, draw there to get points!
                    choiceList.append(i);
                    ////qDebug() << "AI: 1. Adding" << i << "to choices";
                }
            }
        }
    }
    if (!choiceList.isEmpty()) {
        if (Settings::difficulty() == 2) { // to play good ai has to look into the future game
            QList<int> openLines; // list of not yet drawn lines
            for (int i = 0; i < linesSize; i++) {
                if (!lines[i]) {
                    openLines.append(i);
                }
            }
            QList<int> choices = chooseLeastDamaging(openLines); // run extended damage control
            if (!choices.isEmpty()) {
                //qDebug() << "AI: 4. Drawing line at index:" << choices.at(0);
                return choices.at(0);
            }
        }
        float randomFloat = ((float) rand() / (RAND_MAX + 1.0)) * (choiceList.size() - 1);
        int randChoice = (short)(randomFloat) / 1;
        //qDebug() << "AI: 1. Drawing line at index:" << choiceList.at(randChoice);
        return choiceList.at(randChoice);
    }

    choiceList = safeMoves();

    if (choiceList.size() != 0) {
        float randomFloat = ((float) rand() / (RAND_MAX + 1.0)) * (choiceList.size() - 1);
        int randChoice = (short)(randomFloat) / 1;
        //qDebug() << "AI: 2. Drawing line at index:" << choiceList.at(randChoice);
        return choiceList.at(randChoice);
    }

    choiceList.clear();
    for (int i = 0; i < linesSize; i++) { //have to take what's left
        if (!lines[i]) {
            QList<int> adjacentSquares = squaresFromLine(i);
            for (int j = 0; j < adjacentSquares.size(); j++) {

                if (countBorderLines(adjacentSquares.at(j), lines) == 2) { //if 2 lines (they're all that's left!)
                    choiceList.append(i);
                    ////qDebug() << "AI: 3. Adding" << i << "to choices";
                }
            }
        }
    }
    if (Settings::difficulty() >= 1) { //Hard(2/3) //do some damage control :)
        QList<int> goodChoiceList = chooseLeastDamaging(choiceList);
        if (goodChoiceList.size() != 0) {
            float randomFloat = ((float) rand() / (RAND_MAX + 1.0)) * (goodChoiceList.size() - 1);
            int randChoice = (short)(randomFloat) / 1;
            //qDebug() << "AI: 3. Drawing line at index:" << goodChoiceList.at(randChoice);
            return goodChoiceList.at(randChoice);
        }
    }

    if (choiceList.size() != 0) {
        float randomFloat = ((float) rand() / (RAND_MAX + 1.0)) * (choiceList.size() - 1);
        int randChoice = (short)(randomFloat) / 1;
        //qDebug() << "AI: 3. Drawing line at index:" << choiceList.at(randChoice);
        return choiceList.at(randChoice);
    }
    return 0;
}

QList<int> aiController::safeMoves() const
{
    QList<int> safeLines;
    for (int i = 0; i < linesSize; i++) { //finding totally safe moves. avoiding squares with 2 lines
        if (!lines[i]) {
            QList<int> adjacentSquares = squaresFromLine(i);
            int badCount = 0;
            for (int j = 0; j < adjacentSquares.size(); j++) {
                if (countBorderLines(adjacentSquares.at(j), lines) == 2) { //don't want to make 3 lines around a square
                    badCount++;
                }
            }
            if (badCount == 0) {
                safeLines.append(i);
                ////qDebug() << "AI: 2. Adding" << i << "to choices";
            }
        }
    }
    return safeLines;
}

QList<int> aiController::chooseLeastDamaging(const QList<int> &choiceList) const
{
    ////qDebug() << "AI: Checking" << choiceList.size() << "possible moves";
    QMultiMap<int, int> linePointDamage; //this will be a list of how damaging a certain move will be. Key = damage of move, Value = index of line
    QScopedArrayPointer<bool> linesCopy(new bool[linesSize]); //make temporary local copies of lists
    int sidesOfSquare[4];

    QMap<int, QSet<int> > chains; // this is a raw list of chains (which are sets of lines). key = random element of chain
    QMap<int, QSet<int> > chainSet; // same thing as chains but with unique chains
    QList<QList<int> > ownChains; // chains that ai will get in this run. those chains are taken in myLines.
    QList<int> ownMoves; // contains lines of chain that the ai will take first (this will contain the returned move)
    QScopedArrayPointer<bool> myLines(new bool[linesSize]); //make temporary local copies of lists
    int ownLinesCnt = 0; // count of how many lines ai will take in this run
    int ownSquaresCnt = 0; // count of how many squares ai will get in this run

    if (Settings::difficulty() > 1) {
        memcpy(myLines.data(), lines, linesSize); // lines --> myLines (complete own chains) --> linesCopy (analyze damage/chains for next runs)
        bool chainFound;
        // since chooseLeastDamaging will be called early during the game if playing against hard ai, we need to finish open chains in linesCopy before computing the number of residual chains
        do { // this loop completes all chains the opponent gave to ai
            chainFound = false;
            for (int curSquare = 0; curSquare < squareOwners.size(); curSquare++) { // walk through squares and search for start of chain
                QList<int> ownChain; // remember completed chain lines
                int chainSquare = curSquare;
                bool squareFound;
                do { // this loop walks through a chain square by square
                    squareFound = false;
                    if (countBorderLines(sidesOfSquare, chainSquare, &(*myLines)) == 3) { // found a square for ai
                        for (int sideOfSquare = 0; sideOfSquare <= 3; sideOfSquare++) {
                            if (!myLines[sidesOfSquare[sideOfSquare]]) { // found missing line of square
                                ownLinesCnt++;

                                int nextSquareFound = -1;
                                QList<int> adjacentSquares = squaresFromLine(sidesOfSquare[sideOfSquare]);
                                for (int i = 0; i < adjacentSquares.size(); i++) {
                                    int chainSquareBorderCnt = countBorderLines(adjacentSquares.at(i), &(*myLines));
                                    if (chainSquare != adjacentSquares.at(i) &&
                                            chainSquareBorderCnt == 3) { // check if a second square will be completed by this line
                                        ownSquaresCnt++; // add extra square
                                    }
                                    if (chainSquareBorderCnt == 2) { // look for next square in chain
                                        nextSquareFound = adjacentSquares.at(i);
                                    }

                                }
                                myLines[sidesOfSquare[sideOfSquare]] = true; // complete line
                                if (nextSquareFound >= 0) {
                                    chainSquare = nextSquareFound;
                                }
                                ownChain.append(sidesOfSquare[sideOfSquare]);
                            }
                        }
                        squareFound = true;
                        chainFound = true;
                        ownSquaresCnt++;
                    }
                } while (squareFound);
                if (chainFound) {
                    ownChains.append(ownChain);
                    break;
                }
            }
        } while (chainFound);
        ////qDebug() << "ownChains:" << ownChains;

        // complete the shortest chain first if there is more than one chain. this is needed to stop alternating between two chains because that works against the hard ai move which takes the next chain by sacrificing 2/4 squares. when alternating between two chains it's possible that there are 3 remaining open lines in both chains combined which triggers the evil move too late because the chains were completed in the wrong order
        int minChain = -1;
        int tmp = width * height * 10;
        for (int i = 0; i < ownChains.size(); i++) {
            if (tmp > ownChains.at(i).size()) {
                tmp = ownChains.at(i).size();
                minChain = i;
            }
        }
        if (minChain >= 0) {
            ownMoves = ownChains.at(minChain);
        }
        ////qDebug() << "ownMoves:" << ownMoves;
    }

    for (int i = 0; i < choiceList.size(); i++) { //cycle through all the possible moves
        QList<int> squaresCopy = squareOwners;  //make temporary local copies of lists
        QSet<int> chain; // set of lines that are given to opponent by move choiceList.at(i)

        if (Settings::difficulty() > 1) {
            memcpy(linesCopy.data(), myLines.data(), linesSize);    //make temporary local copies of lists
            if (linesCopy[choiceList.at(i)]) {
                continue;    // already covered. ai will get this line
            }
        } else {
            memcpy(linesCopy.data(), lines, linesSize); //make temporary local copies of lists
        }

        linesCopy[choiceList.at(i)] = true; //we're going to try drawing a line here

        //now it would be the next player's turn so we're going to count how many squares they would be able to get.
        int count = 0;  //this is how many points the next player will ge if you draw a line at choiceList.at(i)
        bool squareFound = false;
        chain.insert(choiceList.at(i));
        do {
            for (int currentSquare = 0; currentSquare < squaresCopy.size(); currentSquare++) { //cycle through the grid (by squares):
                if (countBorderLines(sidesOfSquare, currentSquare, &(*linesCopy)) == 3) { //if we've found a square with three sides drawn:
                    count++;
                    squareFound = true; //we found a square so we will look again for the next

                    for (int sideOfSquare = 0; sideOfSquare <= 3; sideOfSquare++) { //make the square complete in linesCopy
                        if (Settings::difficulty() > 1 && !linesCopy[sidesOfSquare[sideOfSquare]]) {
                            chain.insert(sidesOfSquare[sideOfSquare]);
                            QList<int> adjacentSquares = squaresFromLine(sidesOfSquare[sideOfSquare]);
                            for (int adjsq = 0; adjsq < adjacentSquares.size(); adjsq++) {
                                if (currentSquare == adjacentSquares.at(adjsq)) {
                                    continue;
                                }
                                if (countBorderLines(adjacentSquares.at(adjsq), &(*myLines)) == 3) {
                                    // line will complete two squares
                                    count++;
                                }
                            }
                        }
                        linesCopy[sidesOfSquare[sideOfSquare]] = true;  //draw at this squares
                    }   //now current square is completed by the second player.
                    break;  //since we found a square with 3 sides completed (now = 4), we break the 'for(currentSquare)' loop
                } else {
                    squareFound = false;    //we couldn't find a square this time round so we'll try the next 'i'
                }
            }
        } while (squareFound == true);  //while we're still finding squares
        linePointDamage.insert(count, choiceList.at(i));   //insert a pair with Key=count, Value=i
        chains.insert(choiceList.at(i), chain);
    }

    ////qDebug() << "linePointDamage:" << linePointDamage;

    if (Settings::difficulty() < 2) { // middle ai won't analyze the game further
        QList<int> bestMoves = linePointDamage.values(linePointDamage.begin().key());   //this is a list of the indices of the lines that are the least damaging. linePointDamage.begin() returns the 1st pair in the QMap, sorted in ascending order by Key (damage of move)
        return bestMoves;
    }

    ////qDebug() << chains;
    // remove double entries from chains to get chainSet
    QMapIterator<int, QSet<int> > j(chains);
    while (j.hasNext()) { // walk through chains and add chain to chainSet (if not already contained)
        j.next();
        bool newChain = true;
        QSet<int> chainCheck = j.value(); // this is the chain we might add
        QMapIterator<int, QSet<int> > chainSetIt(chainSet);
        while (chainSetIt.hasNext()) { // walk through chainSet and look for chainCheck
            chainSetIt.next();
            QSet<int> chainSetI = chainSetIt.value();
            if (chainSetI == chainCheck) { // found chainCheck in chainSet, don't add
                newChain = false;
                break;
            }
        }
        if (newChain) { // chainCheck not in chainSet
            chainSet.insert(j.key(), chainCheck);
        }
    }
    ////qDebug() << "chainSet:" << chainSet;

    // analyze chains
    // TODO: find loop chains to calculate sacrifices (loopChains are a subset of longChains)
    int shortChains = 0; // chains <= 2 lines
    int longChains = 0; // exploitable chains
    QMapIterator<int, QSet<int> > chainSetIt(chainSet);
    while (chainSetIt.hasNext()) {
        chainSetIt.next();
        QSet<int> chainSetI = chainSetIt.value();
        if (chainSetI.size() <= 3) {
            shortChains++;
        } else {
            longChains++;
        }
    }
    ////qDebug() << "short chains:" << shortChains << ", long chains: " << longChains;

    if (
        (
            (ownLinesCnt == 2) || // sacrifice 2 squares squares to opponent to get next chain.
            (ownLinesCnt == 3 && ownSquaresCnt == 4) // this is for loop chains which require a sacrifice of 4 squares
        )
        &&
        longChains > 0 // only do it if there is at least one chain to steal
        &&
        safeMoves().size() == 0 // only do it in endgames
    ) {
        //qDebug() << "HAHA, our chance to do the evil thing!";
        int magicLine = -1; // line in own moves that is used to get the next chain (draw there to give 2/4 squares to opponent)
        // formal definition of magicLine: line that when completed will leave at least one other line in own moves that completes two squares at once
        // the opposite of magic line will be used in the hard hearted handout to make sure that the opponent won't be able to do the evil move
        for (int i = 0; i < ownMoves.size(); i++) {
            memcpy(myLines.data(), lines, linesSize); // we want to look one line into the future game
            myLines[ownMoves.at(i)] = true; // try ownMove i (one line in chain that ai will get)
            for (int j = 0; j < ownMoves.size(); j++) { // test other lines in own moves
                if (i == j) {
                    continue;
                }
                int leftSquares = 0; // count squares that can be completed by other line (j)
                QList<int> adjacentSquares = squaresFromLine(ownMoves.at(j));
                for (int k = 0; k < adjacentSquares.size(); k++) {
                    if (countBorderLines(adjacentSquares.at(k), &(*myLines)) == 3) {
                        leftSquares++;
                    }
                }
                if (leftSquares == 2) { // we found a line that will yield another line in own moves that completes two squares
                    magicLine = i;
                }
            }
        }
        //qDebug() << "Magic Line index:" << magicLine;
        QList<int> bestMoves;
        if (ownMoves.size() > 1) {
            int ownMove = 1;
            if (magicLine >= 0 && magicLine < ownMoves.size()) {
                ownMove = magicLine;
            }
            bestMoves.append(ownMoves.at(ownMove)); // choose the second line found! in case of 2 squares for ai this will choose the line at the end of the chain. in case of 4 squares this will be the line in the middle, leaving two lines that complete two squares each. FIX: 1 doesn't work in some cases because the algorithm doesn't detect chains by spatial connectedness. ie if there are two ends of a chain the search algorithm can jump between those two ends, messing up the order in ownMoves list. solution is magicLine
            return bestMoves;
        }
    }

    if (ownMoves.size() > 0) { // complete own chain
        QList<int> bestMoves;
        bestMoves.append(ownMoves.at(0));
        return bestMoves;
    }

    if (linePointDamage.begin().key() == 2) { // opponent will get 2 squares
        int handoutLine = -1;
        QList<int> opponentChain;
        QMapIterator<int, QSet<int> > chainSetIt(chainSet);
        while (chainSetIt.hasNext()) {
            chainSetIt.next();
            QSet<int> chainSetI = chainSetIt.value();
            if (chainSetI.contains(linePointDamage.begin().value())) {
                opponentChain = chainSetI.values();
            }
        }
        for (int i = 0; i < opponentChain.size(); i++) {
            memcpy(myLines.data(), lines, linesSize); // we want to look one line into the future game
            myLines[opponentChain.at(i)] = true; // try move in chain for opponent
            for (int j = 0; j < opponentChain.size(); j++) { // test other lines in chain
                if (i == j) {
                    continue;
                }
                int badSquares = 0; // count squares with two open lines (those are dangerous)
                QList<int> adjacentSquares = squaresFromLine(opponentChain.at(j));
                for (int k = 0; k < adjacentSquares.size(); k++) {
                    if (countBorderLines(adjacentSquares.at(k), &(*myLines)) != 3) {
                        badSquares++;
                    }
                }
                if (badSquares == 0) {
                    handoutLine = i;
                }
            }
        }
        if (handoutLine >= 0) {
            ////qDebug() << "Hard hearted handout at" << opponentChain.at(handoutLine);
            QList<int> retMove;
            retMove.append(opponentChain.at(handoutLine));
            return retMove;
        }
    }

    // fallback to middle ai move
    QList<int> bestMoves = linePointDamage.values(linePointDamage.begin().key());   //this is a list of the indices of the lines that are the least damaging. linePointDamage.begin() returns the 1st pair in the QMap, sorted in ascending order by Key (damage of move)
    return bestMoves;
}

int aiController::countBorderLines(int *sidesOfSquare, int squareIndex, const bool *linesList) const
{
    int count = 0;

    linesFromSquare(sidesOfSquare, squareIndex);

    //TODO: replace this with a QList 'count' type function?
    if (linesList[sidesOfSquare[0]] == true) {
        count++;
    }
    if (linesList[sidesOfSquare[1]] == true) {
        count++;
    }
    if (linesList[sidesOfSquare[2]] == true) {
        count++;
    }
    if (linesList[sidesOfSquare[3]] == true) {
        count++;
    }
    ////qDebug() << "AI: Square" << squareIndex << "is bordered by" << count << "lines";
    return count;
}

int aiController::countBorderLines(int squareIndex, const bool *linesList) const
{
    int tempLineList[4];
    return countBorderLines(tempLineList, squareIndex, linesList);
}

QList<int> aiController::squaresFromLine(int lineIndex) const
{
    ////qDebug() << "Line:" << lineIndex;
    QList<int> squareList;
    if (lineDirection(lineIndex) == KSquares::HORIZONTAL) {
        squareList.append(lineIndex - ((width + 1) * (lineIndex / ((width * 2) + 1))));
        squareList.append(squareList.at(0) - width);
        if (squareList.at(1) < 0) {
            squareList.removeAt(1);
        }
        if (squareList.at(0) >= (width * height)) {
            squareList.removeAt(0);
        }

    } else if (lineDirection(lineIndex) == KSquares::VERTICAL) {
        squareList.append(lineIndex - ((lineIndex / ((width * 2) + 1))*width + (lineIndex / ((width * 2) + 1)) + width));
        squareList.append(squareList.at(0) - 1);
        if (lineIndex % ((2 * width) + 1) == width) {
            squareList.removeAt(1);
        }
        if (lineIndex % ((2 * width) + 1) == 2 * width) {
            squareList.removeAt(0);
        }
    }
    ////qDebug() << "Size:" << squareList.size();
    ////qDebug() << "squares:" << squareList.at(0) << " " << squareList.at(1);
    return squareList;
}

void aiController::linesFromSquare(int *linesFromSquare, int squareIndex) const
{
    int index1 = (squareIndex / width) * ((2 * width) + 1) + (squareIndex % width);
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
    int index2 = lineIndex % ((2 * width) + 1);
    KSquares::Direction dir;
    if (index2 < width) {
        dir = KSquares::HORIZONTAL;
    } else {
        dir = KSquares::VERTICAL;
    }

    return dir;
}
