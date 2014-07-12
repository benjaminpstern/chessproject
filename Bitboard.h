#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <stdint.h>
#include "Chessboard.h"
//a class for a representation of a chessboard that has 
class Bitboard : public Chessboard{
private:
	//a int64_t is 8 bytes, enough for 64 squares.
	//this array should be of size 12.
	int64_t* bitbrds;
	//a history of all the moves that have been played.
	move_t* moveHistory;
	//the number of plies(half moves) that have been played
	int plyNo;
	//return a zero-terminated list of moves that must be checked through the isLegal() function
	//before putting it in a list of legal moves
	move_t* semiLegalMoves();
	//returns the index of the board that represents the presence of character c
	//or -1 if the character doesn't represent a piece
	int boardIndex(char c);
	int myPieces(int blackOrWhite){
		return blackOrWhite*6;
	}
	int otherPieces(int blackOrWhite){
		return myPieces((blackOrWhite+1)%2);
	}
public:
	//constructor. Initializes the board to be completely empty.
	Bitboard();
	//returns 1 if the square is occupied, 0 if not,
	// and -1 if x or y are out of bounds
	int isOccupied(uint x, uint y);
	//occupies the square at (x,y) with the piece indicated by the char piece
	//takes the characters KQRNBPkqrnbp
	//returns -1 if x or y are out of bounds
	//and 1 if occupying the square is successful
	int occupy(char piece,uint x,uint y);
	//returns -1 if x or y are out of bounds
	//and 1 if deoccupying the square is successful
	int deoccupy(uint x, uint y);
	//deoccupies a the board of the type of piece pointed to at index
	//returns -1 if x or y are out of bounds
	//and 1 otherwise
	int deoccupy(int index, uint x, uint y);
	//gives a zero-terminated list of legal moves.
	//delete it when done with it.
	move_t* legalMoves();
	//set up the start position
	void startPosition();
	//return true if the move is legal, false otherwise
	bool isLegal(move_t m);
	//make the move on the board, return true if successful
	bool move(move_t m);
	//take back the previous move, return true if successful
	bool takeBack();
	//return the character representing the piece on the board.
	//and if nothing is there return '_'
	char piece(int,int);
	//return a zero-terminated list of all moves that have been played in the game.
	move_t* getMoveHistory();
	int64_t knightSet(int64_t brd,int blackOrWhite);
	int64_t pawnSet(int64_t brd, int blackOrWhite);
	int64_t kingSet(int64_t brd, int blackOrWhite);
	int64_t rookSet(int64_t brd, int blackOrWhite);
	int64_t bishopSet(int64_t brd, int blackOrWhite);
	int64_t queenSet(int64_t brd, int blackOrWhite);
	string tostring();
	~Bitboard();
};
extern ostream& operator<<(ostream &strm, Bitboard* board);