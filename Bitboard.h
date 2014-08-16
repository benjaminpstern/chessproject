#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include "Chessboard.h"
const uint64_t notFirstRank = 0xfefefefefefefefe; // ~0x0101010101010101
const uint64_t notLastRank = 0x7f7f7f7f7f7f7f7f; // ~0x8080808080808080
const uint64_t notFirstSecondRank = ~0x0303030303030303;
const uint64_t notTwoLastRanks = ~0xc0c0c0c0c0c0c0c0;
const uint64_t a1h8Diagonal = 0x8040201008040201;
const uint64_t a8h1Diagonal = 0x102040810204080;
const uint64_t centralSquares = 0x0000001818000000;
/*
 * a class for a representation of a chessboard that has 12 64-bit integers
 * representing the occupancy set for each type of piece.
 * a square (x,y) on this board is represented by the bit at position (8*x+y)
 * where (0,0) is the least significant bit
 */
class Bitboard : public Chessboard{
private:
	//a uint64_t is 8 bytes, enough for 64 squares.
	//this array should be of size 12.
	uint64_t* bitbrds;
	//a history of all the moves that have been played.
	move_t* moveHistory;
	//the number of plies(half moves) that have been played
	int plyNo;
	int nodesSearched;
	//return a zero-terminated list of moves that must be checked through the isLegal() function
	//before putting it in a list of legal moves
	move_t* semiLegalMoves();
	bool isCheck();
	bool isInCheck(int blackOrWhite);
	bool isCheckmate();
	int checkingPieceIndex(int blackOrWhite);
	uint64_t squaresToKing(int blackOrWhite);
	//returns the index of the board that represents the presence of character c
	//or -1 if the character doesn't represent a piece
	int boardIndex(char c);
	uint64_t reverseBits(unsigned char c){
		return (uint64_t)((c * 0x0202020202ULL & 0x010884422010ULL) % 1023);
	}
	//reverses the bits of a 64 bit integer
	uint64_t reverse(uint64_t toReverse){
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
	uint64_t byteSwap(uint64_t toReverse){
		uint64_t inByte0 = (toReverse & 0xFF);
		uint64_t inByte1 = (toReverse & 0xFF00) >> 8;
		uint64_t inByte2 = (toReverse & 0xFF0000) >> 16;
		uint64_t inByte3 = (toReverse & 0xFF000000) >> 24;
		uint64_t inByte4 = (toReverse & 0xFF00000000) >> 32;
		uint64_t inByte5 = (toReverse & 0xFF0000000000) >> 40;
		uint64_t inByte6 = (toReverse & 0xFF000000000000) >> 48;
		uint64_t inByte7 = (toReverse & 0xFF00000000000000) >> 56;
		return (inByte0 << 56) | (inByte1 << 48) | (inByte2 << 40) | (inByte3 << 32) | (inByte4<<24) |(inByte5<<16) | (inByte6<<8)| inByte7;
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
	/*
	 * returns a pointer to an array of null-terminated move objects representing all the legal moves in that position
	 */
	move_t* allMoves();
	/*
	 * returns a pointer to an array of null-terminated move objects representing all the legal moves that can be made by the piece
	 * represented by the int piece
	 */
	move_t* allMoves(int piece);
	//get the x value of the square represented by the 1 bit in brd
	int xValue(uint64_t brd){
		int x=-1;
		while(brd){
			brd>>=8;
			x++;
		}
		return x;
	}
	//1 if white's move, -1 if black's move
	int toMove();
	//the amount of stuff that's hanging.
	//If there's a lot of stuff hanging the program should go deeper to look for the right moves
	int hangingPieces();
	//should get an array with the n best moves.
	move_t* nBestMoves(int n);
	//evaluates the position recursively
	double evaluate();
	double evaluate(int depth);
	double evaluate(int depth, int prevHanging);
	//return the value for the evaluation of the position using the alpha beta pruning evaluation
	double alphaBeta(double alpha, double beta, int depth);
	//goes to the end of all checks and check evasions and some captures. makes sure the position is stable before 
	//evaluating it.
	double quiesce(double alpha, double beta);
	//get the y value of the square represented by the 1 bit in brd
	int yValue(uint64_t brd){
		int y=0;
		while(brd&notFirstRank){
			brd>>=1;
			y++;
		}
		return y;
	}
	//gives the number of pieces on a bitboard
	int numPieces(uint64_t brd){
		int num=0;
		while(brd){
			brd=restPieces(brd);
			num++;
		}
		return num;
	}
	//make the move on the board, return true if successful
	bool move(move_t m);
	//take back the previous move, return true if successful
	bool takeBack();
	//return the character representing the piece on the board.
	//and if nothing is there return '_'
	char piece(int,int);
	//gets the value of the piece on that square
	int pieceValue(int,int);
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
	uint64_t occupancySet();
	uint64_t ownPieces(int blackOrWhite);
	uint64_t enemyPieces(int blackOrWhite);
	uint64_t pieceAttacks(int pieceIndex);
	uint64_t pieceAttacks(int pieceIndex, uint64_t brd);
	uint64_t enemyPieceAttacks(int blackOrWhite);
	uint64_t ownPieceAttacks(int blackOrWhite);
	uint64_t knightAttacks(uint64_t brd,int blackOrWhite);
	uint64_t pawnAttacks(uint64_t brd, int blackOrWhite);
	uint64_t kingAttacks(uint64_t brd, int blackOrWhite);
	uint64_t rookAttacks(uint64_t brd, int blackOrWhite);
	uint64_t bishopAttacks(uint64_t brd, int blackOrWhite);
	uint64_t queenAttacks(uint64_t brd, int blackOrWhite);
	uint64_t firstPiece(uint64_t brd);
	uint64_t restPieces(uint64_t brd);
	uint64_t file(int n){
		return (uint64_t)255<<(8*n);
	}
	uint64_t rank(int n){
		return 0x0101010101010101<<n;
	}
	//returns a diagonal mask excluding the square on square.
	//the diagonal will go up and right if right is true, otherwise up and left
	uint64_t diagonalMask(uint64_t square, bool right){
		if(square==0){//otherwise it will run forever
			return 0;
		}
		uint64_t mask;
		int i=0;
		//print_bitboard(square);
		if(right){
			mask=(uint64_t)1<<8*7;
			while(!(mask&square)){
				//print_bitboard(mask);
				mask>>=8;
				//print_binary(mask);
				//print_bitboard(mask);
				if(i<7){
					mask|=mask<<9;
				}
				i++;
			}
			return mask^square;
		}
		mask=1;
		while(!(mask&square)){
			mask<<=8;
			if(i<7){
				mask|=(mask>>7);
			}
			i++;
		}
		return mask^square;

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


	void print_bitboard(uint64_t brd);
	void print_binary(uint64_t brd);
	string tostring();
	~Bitboard();
};
extern ostream& operator<<(ostream &strm, Bitboard* board);