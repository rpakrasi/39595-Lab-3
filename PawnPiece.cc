#include "PawnPiece.hh"
#include "ChessBoard.hh"

using Student::PawnPiece;
using Student::ChessPiece;
using Student::ChessBoard;

/**
 * Checks if the Paws can move from its current position to (toRow, toColumn).
 * A Pawn moves forward and captures diagonal.
 */

bool PawnPiece::canMoveToLocation(int toRow, int toColumn)
{
    int fromRow = getRow();
    int fromCol = getColumn();

    // same square
    if (fromRow == toRow && fromCol == toColumn) return false;
    const bool white = (getColor() == White);          // White moves up (row - 1)
    const int direction = white ? -1 : 1;
    const int startRow  = white ? (board.getNumRows() - 2) : 1; 

    if (toColumn == fromCol) {
        // one step
        if (toRow == fromRow + direction &&
            board.getPiece(toRow, toColumn) == nullptr) {
            return true;
        }
        if (fromRow == startRow &&
            toRow == fromRow + 2 * direction &&
            board.getPiece(fromRow + direction, fromCol) == nullptr &&
            board.getPiece(toRow, toColumn) == nullptr) {
            return true;
        }
        return false;
    }

    // diagonal capture (exactly one column over, one row forward)
    if (std::abs(toColumn - fromCol) == 1 && toRow == fromRow + direction) {
        ChessPiece* dest = board.getPiece(toRow, toColumn);
        return (dest != nullptr) && (dest->getColor() != getColor());
    }

    return false;
}
