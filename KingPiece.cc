#include "KingPiece.hh"
#include "ChessBoard.hh"
#include <cmath>

using namespace Student;

bool KingPiece::canMoveToLocation(int toRow, int toColumn)
{
    int dRow = std::abs(toRow - row);
    int dCol = std::abs(toColumn - col);

    if (dRow <= 1 && dCol <= 1 && (dRow + dCol > 0))
        return true;

    return false;
}
