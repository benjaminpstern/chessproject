#include "move.h"
#include <iostream>
#include <string>
using namespace std;
//an abstract class for a Chessboard.
//Every implementation of a chessboard should have at least these basic methods
class Chessboard{
public:
	//set up the start position on a board
	virtual void startPosition()=0;
	//tell if a move is legal
	virtual bool isLegal(move_t m)=0;
	//true if one side is in check, otherwise false
	virtual bool isCheck()=0;
	//true if one side is in checkmate, otherwise false
	virtual bool isCheckmate()=0;
	//true if the game is a draw by stalemate, three move repetition, insufficient material, or 50 move rule, otherwise false
	virtual bool isDraw()=0;
	//actually move the piece, returns true if successful
	virtual bool move(move_t m)=0;
	//take back the move, return true if successful
	virtual bool takeBack()=0;
	//return the character corresponding to the piece occupying the square at (x,y)
	//if nothing there return '_'
	virtual char piece(int,int)=0;
	//a zero-terminated history of all moves that have been played in the game
	virtual move_t* getMoveHistory()=0;
	//the only non-abstract method in the class. Returns a string that represents the chessboard
	virtual string tostring()=0;
};