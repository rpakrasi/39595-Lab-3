#include "ChessBoard.hh"
#include "PawnPiece.hh"
#include "RookPiece.hh"
#include "BishopPiece.hh"
#include "KingPiece.hh"
#include "KnightPiece.hh"
#include "QueenPiece.hh"
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
    case King:                                  
        newPiece = new KingPiece(*this, col, startRow, startColumn);
        break;
    case Knight:
        newPiece = new KnightPiece(*this, col, startRow, startColumn);
        break;
    case Queen:
        newPiece = new QueenPiece(*this, col, startRow, startColumn);
        break;
    }

    board.at(startRow).at(startColumn) = newPiece;
}

std::ostringstream ChessBoard::displayBoard()
{
    std::ostringstream outputString;

    outputString << "  ";
    for (int i = 0; i < numCols; i++)
    {
        outputString << i << " ";
    }
    outputString << std::endl << "  ";

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
    if (fromRow < 0 || fromRow >= numRows || fromColumn < 0 || fromColumn >= numCols) return false;
    if (toRow   < 0 || toRow   >= numRows || toColumn   < 0 || toColumn   >= numCols) return false;

    ChessPiece* src = board.at(fromRow).at(fromColumn);
    if (src == nullptr) return false;
    if (fromRow == toRow && fromColumn == toColumn) return false;
    ChessPiece* dst = board.at(toRow).at(toColumn);
    if (dst != nullptr && dst->getColor() == src->getColor()) return false;
    if (!src->canMoveToLocation(toRow, toColumn)) return false;
    ChessPiece* captured = board.at(toRow).at(toColumn);
    board.at(toRow).at(toColumn) = src;
    board.at(fromRow).at(fromColumn) = nullptr;
    src->setPosition(toRow, toColumn);

    bool leavesKingInCheck = isOwnKingInCheck(src->getColor());
    board.at(fromRow).at(fromColumn) = src;
    board.at(toRow).at(toColumn) = captured;
    src->setPosition(fromRow, fromColumn);

    return !leavesKingInCheck;
}

bool ChessBoard::movePiece(int fromRow, int fromColumn, int toRow, int toColumn) 
{ 
    // bounds
    if (fromRow < 0 || fromRow >= numRows || fromColumn < 0 || fromColumn >= numCols) return false;
    if (toRow   < 0 || toRow   >= numRows || toColumn   < 0 || toColumn   >= numCols) return false;

    ChessPiece* src = board.at(fromRow).at(fromColumn);
    if (src == nullptr) return false;
    if ((turn == White && src->getColor() != White) || (turn == Black && src->getColor() != Black)) 
    {
        return false;
    }
    if (!isValidMove(fromRow, fromColumn, toRow, toColumn)) return false;
    bool prevEnPassantAvailable = enPassantAvailable;  // store
    int prevTargetRow = enPassantTargetRow;
    int prevTargetCol = enPassantTargetCol;
    enPassantAvailable = false;  
    PawnPiece* pawn = dynamic_cast<PawnPiece*>(src);

    if (pawn != nullptr)
    {
        int dir = (src->getColor() == White ? -1 : 1);
        if (toRow == fromRow + dir &&
            std::abs(toColumn - fromColumn) == 1 &&
            board.at(toRow).at(toColumn) == nullptr)
        {
            if (prevEnPassantAvailable &&
                toRow == prevTargetRow &&
                toColumn == prevTargetCol)
            {
                int capturedPawnRow = toRow - dir;
                int capturedPawnCol = toColumn;
                ChessPiece* victim = board.at(capturedPawnRow).at(capturedPawnCol);
                if (victim != nullptr)
                {
                    delete victim;
                    board.at(capturedPawnRow).at(capturedPawnCol) = nullptr;
                }
            }
        }
    }
    ChessPiece* captured = board.at(toRow).at(toColumn);
    board.at(toRow).at(toColumn) = src;
    board.at(fromRow).at(fromColumn) = nullptr;
    src->setPosition(toRow, toColumn);
    if (captured)
        delete captured;
    pawn = dynamic_cast<PawnPiece*>(src);
    if (pawn != nullptr)
    {
        int moveDist = std::abs(toRow - fromRow);
        if (moveDist == 2)
        {
            enPassantAvailable = true;
            enPassantTargetRow = (fromRow + toRow) / 2; 
            enPassantTargetCol = fromColumn;
        }
    }
    turn = (turn == White ? Black : White);
    return true;
}

bool ChessBoard::isPieceUnderThreat(int row, int column)  
{ 
    ChessPiece* target = board.at(row).at(column);
    if (target == nullptr) return false;

    Color targetColor = target->getColor();

    for (int r = 0; r < numRows; ++r) 
    {
        for (int c = 0; c < numCols; ++c) 
        {
            ChessPiece* attacker = board.at(r).at(c);
            if (attacker == nullptr) continue;
            if (attacker->getColor() == targetColor) continue; 
            if (r == row && c == column) continue; 
            
            ChessPiece* dest = board.at(row).at(column);
            if (dest != nullptr && dest->getColor() == attacker->getColor()) continue;
            if (attacker->canMoveToLocation(row, column))
            {
                return true;
            }
            
        }
    }
    return false; 
}

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
        if (kingRow != -1) break;
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
                {
                    return true;
                }
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


