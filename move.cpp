#include "move.h"
#define INT_BIT sizeof(int)*CHAR_BIT
using namespace move;
//create a move from (x1,y1) to (x2,y2)
//with move number moveno
//and moving the piece denoted by the character piece
move_t createMove(uint x1, uint y1, uint x2, uint y2, uint moveno, char piece){
	if((x1|y1|x2|y2)>7||moveno>4095){
		return -1;
	}
	move_t move=((((((x1<<3)|y1)<<3)|x2)<<3)|y2)<<(INT_BIT-12);//store the squares and then shift 12 places left to store moveno
	move|=(moveno<<(INT_BIT-12-12));//store moveno
	move|=(uint)piece;//store the character
}
uint x1(move_t move){
	return (move>>(INT_BIT-3))&7;
}
uint y1(move_t move){
	return (move>>(INT_BIT-6))&7;
}
uint x2(move_t move){
	return (move>>(INT_BIT-9))&7;
}
uint y2(move_t move){
	return (move>>(INT_BIT-12))&7;
}
uint moveno(move_t move){
	return (move>>CHAR_BIT)&4095
}
move_t changePiece(move_t move, char c){
	return (move&(UINT_MAX<<CHAR_BIT))|(int)c;
}