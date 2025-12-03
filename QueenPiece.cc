#include "QueenPiece.hh"
#include "ChessBoard.hh"
#include <cmath>

using namespace Student;

bool QueenPiece::canMoveToLocation(int toRow, int toColumn)
{

    if (toRow < 0 || toRow >= board.getNumRows() ||
        toColumn < 0 || toColumn >= board.getNumCols())
        return false;

    ChessPiece* dest = board.getPiece(toRow, toColumn);
    if (dest && dest->getColor() == this->getColor())
        return false;

    int dRow = toRow - row;
    int dCol = toColumn - col;
    int absRow = std::abs(dRow);
    int absCol = std::abs(dCol);

    bool straight = (row == toRow) || (col == toColumn);
    bool diagonal = (absRow == absCol);

    if (!straight && !diagonal)
        return false;

    int stepR = (dRow == 0 ? 0 : (dRow > 0 ? 1 : -1));
    int stepC = (dCol == 0 ? 0 : (dCol > 0 ? 1 : -1));

    int r = row + stepR;
    int c = col + stepC;

    while (r != toRow || c != toColumn) {
        if (board.getPiece(r, c) != nullptr)
            return false;
        r += stepR;
        c += stepC;
    }

    return true;
}
