#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <stdint.h>
#include "Chessboard.h"
const int64_t notFirstRank = 0xfefefefefefefefe; // ~0x0101010101010101
const int64_t notLastRank = 0x7f7f7f7f7f7f7f7f; // ~0x8080808080808080
const int64_t notFirstSecondRank = ~0x0202020202020202;
const int64_t notTwoLastRanks = ~0xc0c0c0c0c0c0c0c0;
/*
 * a class for a representation of a chessboard that has 12 64-bit integers
 * representing the occupancy set for each type of piece.
 * a square (x,y) on this board is represented by the bit at position (8*x+y)
 * where (0,0) is the least significant bit
 */
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
	int64_t reverseBits(unsigned char c){
		return (int64_t)(c * 0x0202020202ULL & 0x010884422010ULL) % 1023;
	}
	//reverses the bits of a 64 bit integer
	int64_t reverse(int64_t toReverse){
		unsigned int reversed;
		unsigned char inByte0 = (toReverse & 0xFF);
		unsigned char inByte1 = (toReverse & 0xFF00) >> 8;
		unsigned char inByte2 = (toReverse & 0xFF0000) >> 16;
		unsigned char inByte3 = (toReverse & 0xFF000000) >> 24;
		unsigned char inByte4 = (toReverse & 0xFF00000000) >> 32;
		unsigned char inByte5 = (toReverse & 0xFF0000000000) >> 40;
		unsigned char inByte6 = (toReverse & 0xFF000000000000) >> 48;
		unsigned char inByte7 = (toReverse & 0xFF00000000000000) >> 56;
		return (reverseBits(inByte0) << 56) | (reverseBits(inByte1) << 48) | (reverseBits(inByte2) << 40) | (reverseBits(inByte3) << 32| 
			(reverseBits(inByte4))<<24 |(reverseBits(inByte5))<<16 | (reverseBits(inByte6))<<8| reverseBits(inByte7));
	}
	/*
	 * blackOrWhite is an integer that is 0 if a piece is white and 1 if the piece is black
	 * returns the index of the first piece of that color in the pieces array
	 */
	int myPieces(int blackOrWhite){
		return blackOrWhite*6;
	}
	//returns the index of the first piece of the enemy color
	int otherPieces(int blackOrWhite){
		return ((blackOrWhite+1)%2)*6;
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
	/*
	 * the following set methods generate 64 bit integers that represent movement sets and occupancy sets.
	 * occupancySet, ownPieces, enemyPieces are occupancy sets of all pieces and pieces of a specific color
	 * pieceSet is a moveset for a particular piece type on the board
	 * knightSet, pawnSet, kingSet, rookSet, bishopSet, and queenSet are the sets of all legal moves
	 * that can be made with that type of piece
	 * firstPiece isolates the least significant bit on a board. Useful for finding the moves of this piece for move generation
	 * file and rank are masks that are all 1's on their specific file or rank, for example the 0th file is the A file.
	 */
	int64_t occupancySet();
	int64_t ownPieces(int blackOrWhite);
	int64_t enemyPieces(int blackOrWhite);
	int64_t pieceAttacks(int pieceIndex);
	int64_t knightAttacks(int64_t brd,int blackOrWhite);
	int64_t pawnAttacks(int64_t brd, int blackOrWhite);
	int64_t kingAttacks(int64_t brd, int blackOrWhite);
	int64_t rookAttacks(int64_t brd, int blackOrWhite);
	int64_t bishopAttacks(int64_t brd, int blackOrWhite);
	int64_t queenAttacks(int64_t brd, int blackOrWhite);
	int64_t firstPiece(int64_t brd);
	int64_t restPieces(int64_t brd);
	int64_t file(int n){
		return (int64_t)255<<(8*n);
	}
	int64_t rank(int n){
		return 0x0101010101010101<<n;
	}
	/*
	 * These are print methods for debugging and testing purposes
	 * print bitboard and print binary print different representations of a single 64-bit integer
	 *
	 * tostring prints the entire board with a representation similar to this:
	 */

	 //  r _ _ _ k b _ r 
	 //	 p p q n p p p p		
	 //  _ _ p _ _ n _ _
	 //  _ _ _ _ _ b _ _
	 //  P _ N P _ _ _ _
	 //  _ _ N _ _ _ _ _
	 //  _ P _ _ P P P P
	 //  R _ B Q K B _ P


	void print_bitboard(int64_t brd);
	void print_binary(int64_t brd);
	string tostring();
	~Bitboard();
};
extern ostream& operator<<(ostream &strm, Bitboard* board);