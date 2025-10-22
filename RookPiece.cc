#include "RookPiece.hh"
#include "ChessBoard.hh"

using Student::RookPiece;
using Student::ChessPiece;
using Student::ChessBoard;

/**
 * Checks if the Rook can move from its current position to (toRow, toColumn).
 * A Rook moves horizontally or vertically any number of squares without jumping over pieces.
 */

bool RookPiece::canMoveToLocation(int toRow, int toColumn)
{
    int fromRow = getRow();
    int fromColumn = getColumn();

    if (fromRow == toRow && fromColumn == toColumn)
    {
        return false;
    }

    //move in a straight line, same row OR same column
    if (fromRow != toRow && fromColumn != toColumn)
    {
        return false;
    }

    //step direction
    int rowStep = (toRow > fromRow) ? 1 : (toRow < fromRow ? -1 : 0);
    int colStep = (toColumn > fromColumn) ? 1 : (toColumn < fromColumn ? -1 : 0);

    //check for pieces in between
    int r = fromRow + rowStep;
    int c = fromColumn + colStep;
    while (r != toRow || c != toColumn)
    {
        if (board.getPiece(r, c) != nullptr)
        {
            return false;
        }
        r += rowStep;
        c += colStep;
    }

    //destination
    ChessPiece *destPiece = board.getPiece(toRow, toColumn);
    if (destPiece == nullptr)
    {
        return true;
    }

    return destPiece->getColor() != getColor();
}
