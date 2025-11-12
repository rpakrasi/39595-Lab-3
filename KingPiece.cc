#include "KingPiece.hh"
#include "ChessBoard.hh"
#include <cmath>

using namespace Student;

bool KingPiece::canMoveToLocation(int toRow, int toColumn)
{
    if (toRow == getRow() && toColumn == getColumn())
        return false;

    if (toRow < 0 || toRow >= board.getNumRows() ||
        toColumn < 0 || toColumn >= board.getNumCols())
        return false;
    int dr = std::abs(toRow - getRow());
    int dc = std::abs(toColumn - getColumn());
    if (dr > 1 || dc > 1)
        return false;
    ChessPiece *target = board.getPiece(toRow, toColumn);
    if (target && target->getColor() == getColor())
        return false;
    return true;
}
