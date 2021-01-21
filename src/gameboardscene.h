/*
    SPDX-FileCopyrightText: 2006 Matthew Williams <matt@milliams.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef GAMEBOARDSCENE_H
#define GAMEBOARDSCENE_H

#include <QGraphicsScene>

class QGraphicsEllipseItem;

/**
 * @short Scene for displaying the game board
 *
 * Created anew at the beginning of each game.
 * Contains the visual representation of the board. Is used to interperet mouse clicks from the user (if the view is unlocked)
 * Contains QGraphicsLineItems for lines, QGraphicsEllipseItems for the dots and QGraphicsRectItem for when a square is complete.
 *
 * @author Matt Williams <matt@milliams.com>
 */
class GameBoardScene : public QGraphicsScene
{
    Q_OBJECT

public:
    /**
     * Create a new gameboard scene with the appropriate size.
     * @param newWidth the number of squares wide the board is
     * @param newHeight the number of squares tall the board is
     * @param parent passed to QGraphicsScene's constructor
     */
    GameBoardScene(int newWidth, int newHeight, QObject *parent = nullptr);
    ///Destructor
    ~GameBoardScene() override;
    /**
     * The smallest the view can be when 'auto-zoom' is off
     * @return the minimum size the view should be
     */
    const QSize minimumSizeHint() const;

    void acknowledgeMove(int x1, int y1, int x2, int y2);

public Q_SLOTS:
    /**
     * Add the line to the scene so it shows up in the view
     * @param index the line-index of the line
     * @param colour the colour of the line
     */
    void drawLine(int index, const QColor &colour);
    /**
      * Draw a temporary halo behind a line.
      * @param index the line-index of the line
      */
    void highlightLine(int index);
    /**
     * Fill a box to show it is owned be a particular player.
     * @param index the square-index of the square
     * @param colour the colour fill of the square
     */
    void drawSquare(int index, const QColor &colour);
    /// disables mouse events
    void enableEvents()
    {
        acceptEvents = true;
    }
    /// enables mouse events
    void disableEvents()
    {
        acceptEvents = false;
    }

protected:
    /**
     * Given a single location in the scene, gives the two nearest QGraphicsEllipseItem
     * @param pos the point in question
     * @return QList of (hopefully 2) QGraphicsEllipseItem*s
     */
    QList<QGraphicsEllipseItem *> getTwoNearestPoints(const QPointF &pos) const;

    /**
     * Given a pair of points, returns whether there is already a line there.
     * @param pointPair QList of (hopefully 2) QGraphicsEllipseItem*s
     * @return trur if there is a line there
     */
    bool isLineAlready(const QList<QGraphicsEllipseItem *> &pointPair) const;
    /**
     * Adds the line to the index for a specified pair of points.
     * @param pointPair QList of (hopefully 2) QGraphicsEllipseItem*s
     */
    void addLineToIndex(const QList<QGraphicsEllipseItem *> &pointPair);

    //conversion functions
    /**
     * Takes a pair of QGraphicsEllipseItems and finds the index that relates to the line that's between them
     * @param pointPair QList of (hopefully 2) QGraphicsEllipseItem*s
     * @return the line-index
     */
    int indexFromPointPair(const QList<QGraphicsEllipseItem *> &pointPair) const;   //given a pointPair, returns the index of the line between them. If not a valid line, returns -1
    /**
     * Takes a line-index and returns a QLineF located at that position
     * @param index the line-index
     * @return line located at the correct position
     */
    QLineF lineFromIndex(int index) const;  //all external calls will need to be passed through this to convert to local coords

    ///Moves to show where the next line will be drawn
    QGraphicsLineItem *indicatorLine;

    ///A local list of lines (non-canon)
    QList<bool> lineList;   //Just kept in sync with the KSquaresGame one

    ///QGraphicsEllipseItem::type()
    int QGraphicsEllipseItemType;
    ///Width of the board
    int width;
    ///Height of the board
    int height;
    ///Pixel spacing for standard zoom
    int spacing;
    ///This property holds whether mouse events are enabled for this widget.
    bool acceptEvents;

    //event handlers
    //void mousePressEvent (QGraphicsSceneMouseEvent* mouseEvent);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) override;

Q_SIGNALS:
    ///Emits the index of the closet (undrawn) line when a click is detected
    void lineDrawn(int);
};

#endif // GAMEBOARDSCENE_H
