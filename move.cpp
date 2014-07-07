#include "move.h"
#define INT_BIT sizeof(int)*CHAR_BIT
using namespace move;
move_t createMove(uint x1, uint y1, uint x2, uint y2, uint moveno, char piece){
	if((x1|y1|x2|y2)>7||moveno>1023){
		return -1;
	}
	move_t move=((((((x1<<3)|y1)<<3)|x2)<<3)|y2)<<(INT_BIT-12);
	move|=(moveno<<(INT_BIT-12-10));
	move|=(uint)piece;
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
move_t changePiece(move_t move, char c){
	return (move&UINT_MAX<<CHAR_BIT)|(int)c;
}