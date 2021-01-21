/*
    SPDX-FileCopyrightText: 2006 Matthew Williams <matt@milliams.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef KSQUARESGAME_H
#define KSQUARESGAME_H

#include <QObject>
#include <QVector>

#include "ksquaresplayer.h"

class QColor;

/**
 * @short The game controller
 *
 * Keeps charge of the game. Everything you'd expect really.
 *
 * - Create one instance of this class which will last the whole time the program is running.
 *
 * - In order to start a (new) game just call @ref createGame() with the appropriate arguments.
 *
 * - Once the game is started, play passes to the first player. @ref takeTurnSig() will be emitted to allow you to chose how the go should be taken (AI controller or 'click' from a View).
 *
 * - However the turn is taken, to make the move, the @ref addLineToIndex() function must be called. This will emit the @ref drawLine() signal to allow you to draw the line on the board with the correct colours.
 *
 * - If the player completed a square, @ref drawSquare() will then be emitted to allow you to draw the completed square with the correct colour.
 *
 * - If the player gets another go, @ref takeTurnSig() will be emitted again. If not, play will pass to the next player and @ref takeTurnSig() will be emitted for them.
 *
 * - If a player completes the scoreboard, @ref gameOver() will be emitted with the full list of players to allow you to construct a score board.
 *
 * - All variables will remain in the state they were at the end of the game until @ref createGame() is called again.
 *
 * @author Matt Williams <matt@milliams.com>
 */

class KSquaresGame : public QObject
{
    Q_OBJECT

public:
    ///Constructor
    KSquaresGame();
    ~KSquaresGame() override;

    /**
     * Create a new game
     *
     * @param startPlayers liat of the players in the game
     * @param startWidth the width of the game board
     * @param startHeight the height of the game board
     */
    void createGame(const QVector<KSquaresPlayer> &startPlayers, int startWidth, int startHeight);
    /**
     * Starts the game
     */
    void start()
    {
        gameInProgress = true;
    }
    /**
     * Stops the game
     */
    void stop()
    {
        gameInProgress = false;
    }
    /**
     * Externally determined player switch, for network game
     */
    void switchPlayer();
    /**
     * @return the id of the current player. 0 >= id \< number of players
     */
    int currentPlayerId() const
    {
        return i_currentPlayerId;
    }
    /**
     * @return the current player
     */
    KSquaresPlayer *currentPlayer()
    {
        return &players[currentPlayerId()];
    }

    //getters
    /**
     * @return the table of currently owned squares
     */
    QList<int> squares() const
    {
        return squareOwnerTable;
    }
    /**
     * @return the list of lines
     */
    QList<bool> lines() const
    {
        return lineList;
    }
    /**
     * @return the width of the game board
     */
    int boardWidth() const
    {
        return width;
    }
    /**
     * @return the height of the game board
     */
    int boardHeight() const
    {
        return height;
    }

public Q_SLOTS:
    /**
     * @param index the index of the line to add
     */
    void addLineToIndex(int index);

protected:
    /**
     * Moves play control to the next player, looping round when necessary
     *
     * @return the Id of the player who's turn just started
     */
    int nextPlayer();
    ///Sets lots of things to zero, clears lists etc.
    void resetEverything();

    ///A line was drawn, see if the player gets another go
    void tryEndGo();
    ///Scans the board to see if any new squares were completed
    void checkForNewSquares();
    /**
     * A player completed a square. Emits the lineDrawn() signal. Checks to see if the game is over.
     *
     * @param index the index of the square which was completed
     */
    void playerSquareComplete(int index);

    // Static throughout each game
    ///Number of players in this game
    int numOfPlayers;
    ///Width of the game board
    int width;
    /// Height of the game board
    int height;
    ///List of the squares and their owners
    QList<int> squareOwnerTable;
    ///List of the lines and whether they're drawn
    QList<bool> lineList;

    // Updated as the game progresses
    ///List of all the players in the game
    QVector<KSquaresPlayer> players;

    // Probably changes every go
    ///Id of the current player
    int i_currentPlayerId;
    /// Should the current player have another go
    bool anotherGo;
    /// is there currently a game in progress
    bool gameInProgress;
    /// last line added
    int lastLine;
Q_SIGNALS:
    ///A player's turn has started. This allows you to use AI/networking etc.
    void takeTurnSig(KSquaresPlayer *); //emit the new curent player
    ///emitted when the game board is completed. Allows you to construct a scoreboard
    void gameOver(const QVector<KSquaresPlayer> &); //for scoreboard purposes
    ///Emits the index and colour of the line
    void drawLine(int, QColor); //int == lineList index
    ///Emits the index and colour of the square
    void drawSquare(int, QColor);   //int == squareOwnerTable index
    ///Emitted when the last move in a series is played by the AI
    void highlightMove(int);
};

#endif // KSQUARESGAME_H
