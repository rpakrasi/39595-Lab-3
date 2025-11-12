#include "KingPiece.hh"
#include "ChessBoard.hh"
#include <cmath>

using namespace Student;

bool KingPiece::canMoveToLocation(int toRow, int toColumn)
{
    if (toRow < 0 || toRow >= board.getNumRows() ||
        toColumn < 0 || toColumn >= board.getNumCols())
        return false;

    int dRow = std::abs(toRow - row);
    int dCol = std::abs(toColumn - col);
    if (dRow <= 1 && dCol <= 1 && (dRow + dCol > 0))
        return true;

    return false;
}
