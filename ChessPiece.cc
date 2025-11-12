#include "ChessPiece.hh"
#include "ChessBoard.hh"

// Include derived headers so RTTI can identify them
#include "PawnPiece.hh"
#include "RookPiece.hh"
#include "BishopPiece.hh"
#include "KingPiece.hh"

using namespace Student;

ChessPiece::ChessPiece(ChessBoard &b, Color c, int r, int c2)
    : board(b), color(c), row(r), col(c2) {}

Type ChessPiece::getType()
{
  if (dynamic_cast<const PawnPiece*>(this))   return Pawn;
  if (dynamic_cast<const RookPiece*>(this))   return Rook;
  if (dynamic_cast<const BishopPiece*>(this)) return Bishop;
  if (dynamic_cast<const KingPiece*>(this))   return King;
  return Pawn;
}
Color ChessPiece::getColor() { return color; }
int ChessPiece::getRow() { return row; }
int ChessPiece::getColumn() { return col; }
void ChessPiece::setPosition(int r, int c2) {
  row = r;
  col = c2;
}
