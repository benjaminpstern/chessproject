#include "move.h"
#define INT_BIT sizeof(int)*CHAR_BIT
//create a move from (x1,y1) to (x2,y2)
//moving piece denoted by pieceMoved, 
//and taking the piece denoted by the character pieceTaken
move_t::move_t(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, char pieceMoved, char pieceTaken){
	this->x1=x1;
	this->y1=y1;
	this->x2=x2;
	this->y2=y2;
	this->pieceMoved=indexPiece(pieceMoved);
	this->pieceTaken=indexPiece(pieceTaken);
}
