#include "ChessPiece.hh"
#include "ChessBoard.hh"

// Include derived headers so RTTI can identify them
#include "PawnPiece.hh"
#include "RookPiece.hh"
#include "BishopPiece.hh"
#include "KingPiece.hh"

using namespace Student;

Type ChessPiece::getType()
{
  if (dynamic_cast<const PawnPiece*>(this))   return Pawn;
  if (dynamic_cast<const RookPiece*>(this))   return Rook;
  if (dynamic_cast<const BishopPiece*>(this)) return Bishop;
  if (dynamic_cast<const KingPiece*>(this))   return King;
  // Fallback (should never happen in our setup)
  return Pawn;
}
