#include "ChessBoard.hh"
#include "PawnPiece.hh"
#include "RookPiece.hh"
#include "BishopPiece.hh"
#include "KingPiece.hh"

using Student::ChessBoard;


ChessBoard::ChessBoard(int numRow, int numCol)
    : numRows(numRow), numCols(numCol)
{
    board.resize(numRows, std::vector<ChessPiece *>(numCols, nullptr));
}

void ChessBoard::createChessPiece(Color col, Type ty, int startRow, int startColumn)
{
    if (board.at(startRow).at(startColumn) != nullptr)
    {
        delete board.at(startRow).at(startColumn);
        board.at(startRow).at(startColumn) = nullptr;
    }

    ChessPiece *newPiece = nullptr;

    switch (ty)
    {
    case Pawn:
        newPiece = new PawnPiece(*this, col, startRow, startColumn);
        break;
    case Rook:
        newPiece = new RookPiece(*this, col, startRow, startColumn);
        break;
    case Bishop:
        newPiece = new BishopPiece(*this, col, startRow, startColumn);
        break;
    }

    board.at(startRow).at(startColumn) = newPiece;
}

std::ostringstream ChessBoard::displayBoard()
{
    std::ostringstream outputString;
    // top scale
    outputString << "  ";
    for (int i = 0; i < numCols; i++){
        outputString << i << " ";
    }
    outputString << std::endl << "  ";
    // top border
    for (int i = 0; i < numCols; i++){
        outputString << "– ";
    }
    outputString << std::endl;

    for (int row = 0; row < numRows; row++){
        outputString << row << "|";
        for (int column = 0; column < numCols; column++){
            ChessPiece *piece = board.at(row).at(column);
            outputString << (piece == nullptr ? " " : piece->toString()) << " ";
        }
        outputString << "|" << std::endl;
    }

    // bottom border
    outputString << "  ";
    for (int i = 0; i < numCols; i++){
        outputString << "– ";
    }
    outputString << std::endl << std::endl;

    return outputString;
}

bool ChessBoard::isValidMove(int fromRow, int fromColumn, int toRow, int toColumn)
{
    // Bounds
    if (fromRow < 0 || fromRow >= numRows || fromColumn < 0 || fromColumn >= numCols) return false;
    if (toRow   < 0 || toRow   >= numRows || toColumn   < 0 || toColumn   >= numCols) return false;

    // Must have a piece at source
    ChessPiece* src = board.at(fromRow).at(fromColumn);
    if (src == nullptr) return false;

    // No moving to same square
    if (fromRow == toRow && fromColumn == toColumn) return false;

    // Final square cannot hold same-color piece
    ChessPiece* dst = board.at(toRow).at(toColumn);
    if (dst != nullptr && dst->getColor() == src->getColor()) return false;

    // Delegate to piece-specific logic
    return src->canMoveToLocation(toRow, toColumn);
}

// Stubs for Part 1 so you can link (real logic needed in Part 2/3)
bool ChessBoard::movePiece(int, int, int, int) { return false; }
bool ChessBoard::isPieceUnderThreat(int, int)  { return false; }

