/*
    SPDX-FileCopyrightText: 2006 Matthew Williams <matt@milliams.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef KSQUARESPLAYER_H
#define KSQUARESPLAYER_H

#include <QColor>

/**
 * @short Player class for KSquares
 *
 * Class containing All the information about a player.
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
     * @param newColor the colour used to fill the player's boxes
     * @param isPlayerHuman is the player human (or AI)
     */
    explicit KSquaresPlayer(const QString &newName, const QColor &newColor, bool isPlayerHuman = true)
    {
        setName(newName);    //defaults to human player
        playerColour = newColor;
        human = isPlayerHuman;
        playerScore = 0;
    }

    ///Create a null player
    KSquaresPlayer() {} //Needed to do QVector<KSquaresPlayer>::operator[int i] since it has to allocate memory i think

    /**
     * Sets the players name
     *
     * @param newName The displayable name of the player
     */
    void setName(const QString &newName)
    {
        playerName = newName;
    }

    /**
     * Sets the players name
     *
     * @return The displayable name of the player
     */
    QString name() const
    {
        return playerName;
    }

    /**
     * Sets the players score
     *
     * @param newScore The players new score
     */
    void setScore(int newScore)
    {
        playerScore = newScore;
    }
    /**
     * @return The players current score
     */
    int score() const
    {
        return playerScore;
    }
    ///Increase the players score by 1
    void incScore()
    {
        playerScore++;
    }

    /**
     * @return Is the player human
     */
    bool isHuman() const
    {
        return human;
    }

    /**
     * @return the player's chosen display colour
     */
    QColor colour() const
    {
        return playerColour;
    }

    /**
     * @return the player's signature (1 or 2 letters)
     */
    QString sig() const
    {
        return displaySig;
    }

    /**
     * Used in sort algorithm. Compares the player's score (not their height or age or anything :D)
     * @param player the player to compare wth this one
     * @return true if player is bigger than this
     */
    bool operator<(const KSquaresPlayer &player) const
    {
        return score() < player.score();
    }
    bool operator>(const KSquaresPlayer &player) const
    {
        return score() > player.score();
    }

protected:
    /// Is the player a human
    bool human;
    /// Player's current score
    int playerScore;
    /// The player's displayable name
    QString playerName;
    ///Player's display colour
    QColor playerColour;
    ///Letter (or two) to display on the board
    QString displaySig;
};

#endif // KSQUARESPLAYER_H
