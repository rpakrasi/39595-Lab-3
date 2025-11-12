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
    case King:                                       // ✅ ← ADD THIS
        newPiece = new KingPiece(*this, col, startRow, startColumn);
        break;
    }

    board.at(startRow).at(startColumn) = newPiece;
}

std::ostringstream ChessBoard::displayBoard()
{
    std::ostringstream outputString;
    
    // top scale
    outputString << "  ";
    for (int i = 0; i < numCols; i++)
    {
        outputString << i << " ";
    }
    outputString << std::endl << "  ";
    
    // top border
    for (int i = 0; i < numCols; i++)
    {
        outputString << "– ";
    }
    outputString << std::endl;

    for (int row = 0; row < numRows; row++)
    {
        outputString << row << "|";
        for (int column = 0; column < numCols; column++)
        {
            ChessPiece *piece = board.at(row).at(column);
            outputString << (piece == nullptr ? " " : piece->toString()) << " ";
        }
        outputString << "|" << std::endl;
    }

    // bottom border
    outputString << "  ";
    for (int i = 0; i < numCols; i++)
    {
        outputString << "– ";
    }
    outputString << std::endl << std::endl;

    return outputString;
}

bool ChessBoard::isValidMove(int fromRow, int fromColumn, int toRow, int toColumn)
{
    // bounds
    if (fromRow < 0 || fromRow >= numRows || fromColumn < 0 || fromColumn >= numCols) return false;
    if (toRow   < 0 || toRow   >= numRows || toColumn   < 0 || toColumn   >= numCols) return false;

    // must have a piece at source
    ChessPiece* src = board.at(fromRow).at(fromColumn);
    if (src == nullptr) return false;

    // no moving to same square
    if (fromRow == toRow && fromColumn == toColumn) return false;

    // final square cannot hold same-color piece
    ChessPiece* dst = board.at(toRow).at(toColumn);
    if (dst != nullptr && dst->getColor() == src->getColor()) return false;

    // delegate to piece-specific logic
    return src->canMoveToLocation(toRow, toColumn);
}

bool ChessBoard::movePiece(int fromRow, int fromColumn, int toRow, int toColumn) 
{ 
    // bounds
    if (fromRow < 0 || fromRow >= numRows || fromColumn < 0 || fromColumn >= numCols) return false;
    if (toRow   < 0 || toRow   >= numRows || toColumn   < 0 || toColumn   >= numCols) return false;

    ChessPiece* src = board.at(fromRow).at(fromColumn);
    if (src == nullptr) return false;// no piece at source

    // enforce turn (White starts)
    if ((turn == White && src->getColor() != White) || (turn == Black && src->getColor() != Black)) 
    {
        return false;
    }

    // validate move
    if (!isValidMove(fromRow, fromColumn, toRow, toColumn)) return false;

    ChessPiece* captured = board.at(toRow).at(toColumn);
    board.at(toRow).at(toColumn) = src;
    board.at(fromRow).at(fromColumn) = nullptr;
    src->setPosition(toRow, toColumn);

    // check if own King is now under attack
    bool inCheck = isOwnKingInCheck(src->getColor());

    if (inCheck)
    {
        // revert the simulated move
        board.at(fromRow).at(fromColumn) = src;
        board.at(toRow).at(toColumn) = captured;
        src->setPosition(fromRow, fromColumn);
        return false;
    }
    if (captured)
        delete captured;

    // switch turn
    turn = (turn == White ? Black : White);
    return true;
}

bool ChessBoard::isPieceUnderThreat(int row, int column)  
{ 
    // must be a piece there
    ChessPiece* target = board.at(row).at(column);
    if (target == nullptr) return false;

    Color targetColor = target->getColor();

    // check every opponent piece to check valid capture onto (row, column)
    for (int r = 0; r < numRows; ++r) 
    {
        for (int c = 0; c < numCols; ++c) 
        {
            ChessPiece* attacker = board.at(r).at(c);
            if (attacker == nullptr) continue;
            if (attacker->getColor() == targetColor) continue; // same side

            // isValidMove ignores turn and checks path/obstructions/capture
            if (isValidMove(r, c, row, column)) 
            {
                return true;
            }
        }
    }
    return false; 
}
#include "ChessBoard.hh"
#include "KingPiece.hh"
#include <cmath>
using namespace Student;

bool ChessBoard::isOwnKingInCheck(Color color)
{
    int kingRow = -1;
    int kingCol = -1;
    for (int r = 0; r < numRows; ++r)
    {
        for (int c = 0; c < numCols; ++c)
        {
            ChessPiece *p = board.at(r).at(c);
            if (p && p->getColor() == color && p->getType() == King)
            {
                kingRow = r;
                kingCol = c;
                break;
            }
        }
    }

    if (kingRow == -1)
        return false; 

    for (int r = 0; r < numRows; ++r)
    {
        for (int c = 0; c < numCols; ++c)
        {
            ChessPiece *attacker = board.at(r).at(c);
            if (attacker && attacker->getColor() != color)
            {
                if (attacker->canMoveToLocation(kingRow, kingCol))
                    return true;
            }
        }
    }
    return false;
}

Student::ChessBoard::~ChessBoard() {
    for (auto &row : board) {
        for (auto &cell : row) {
            delete cell;
            cell = nullptr;
        }
    }
}


