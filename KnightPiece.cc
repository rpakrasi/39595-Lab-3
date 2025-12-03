#include "KnightPiece.hh"
#include "ChessBoard.hh"
#include <cmath>

using namespace Student;

bool KnightPiece::canMoveToLocation(int toRow, int toColumn)
{
    // boundary check
    if (toRow < 0 || toRow >= board.getNumRows() ||
        toColumn < 0 || toColumn >= board.getNumCols())
        return false;


    ChessPiece* dest = board.getPiece(toRow, toColumn);
    if (dest && dest->getColor() == this->getColor())
        return false;

    int dRow = std::abs(toRow - row);
    int dCol = std::abs(toColumn - col);

    return (dRow == 2 && dCol == 1) ||
           (dRow == 1 && dCol == 2);
}
