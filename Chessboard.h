#include "move.h"
#include <iostream>
#include <string>
using namespace std;
class Chessboard{
public:
	//set up the start position on a board
	virtual void startPosition()=0;
	//tell if a move is legal
	virtual bool isLegal(move_t m)=0;
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