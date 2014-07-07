#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include "move.h"
#define INT_BIT sizeof(int)*CHAR_BIT
const uint bboard_size=64/CHAR_BIT;
class Bitboard{
private:
	char piece;
	long long squares;
public:
	Bitboard(char c);
	char getPiece(){
		return piece;
	}
	void setSquares(long long newSquares){
		squares=newSquares;
	}
	//returns 1 if the square is occupied, 0 if not,
	// and -1 if x or y are out of bounds
	int isOccupied(uint x, uint y);
	//returns -1 if x or y are out of bounds
	//and 1 if occupying the square is successful
	int occupy(uint x,uint y);
	//returns -1 if x or y are out of bounds
	//and 1 if deoccupying the square is successful
	int deoccupy(uint x, uint y);
	Bitboard* knightSet();
	Bitboard* pawnSet();
	Bitboard* kingSet();
	Bitboard* rookSet();
	Bitboard* bishopSet();
	Bitboard* queenSet();
	~Bitboard();
};
