#include "BishopPiece.hh"
#include "ChessBoard.hh"

using Student::BishopPiece;
using Student::ChessPiece;
using Student::ChessBoard;

/**
 * Checks if the Bishop can move from its current position to (toRow, toColumn).
 * A Bishop moves diagonally any number of squares without jumping over pieces.
 */

bool BishopPiece::canMoveToLocation(int toRow, int toColumn)
{
    int fromRow = getRow();
    int fromColumn = getColumn();

    //cannot move to same spot
    if (fromRow == toRow && fromColumn == toColumn)
    {
        return false;
    }

    //move diagonally - difference in rows == difference in columns
    int rowDiff = abs(toRow - fromRow);
    int colDiff = abs(toColumn - fromColumn);

    if (rowDiff != colDiff)
    {
        return false;
    }

    //step direction
    int rowStep = (toRow > fromRow) ? 1 : -1;
    int colStep = (toColumn > fromColumn) ? 1 : -1;

    //pieces along the diagonal path
    int r = fromRow + rowStep;
    int c = fromColumn + colStep;
    while (r != toRow && c != toColumn)
    {
        if (board.getPiece(r, c) != nullptr)
        {
            return false;
        }
        r += rowStep;
        c += colStep;
    }

    //destination must be empty or have opponent piece
    ChessPiece *destPiece = board.getPiece(toRow, toColumn);
    if (destPiece == nullptr)
    {
        return true;
    }

    return destPiece->getColor() != getColor();
}
