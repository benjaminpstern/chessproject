
#include "Bitboard.h"
#include <iostream>
#include <bitset>
Bitboard::Bitboard(char c){
	piece=c;
	squares=0;
}
//returns 1 if the square is occupied, 0 if not,
// and -1 if x or y are out of bounds
int Bitboard::isOccupied(uint x, uint y){
	if((x|y)>7){
		return -1;
	}
	//std::cout<< piece<<std::bitset<64> (squares)<<"\n";
	return 1&((squares>>(x*CHAR_BIT))>>y);
}
//returns -1 if x or y are out of bounds
//and 1 if occupying the square is successful
int Bitboard::occupy(uint x,uint y){
	if((x|y)>7){
		return -1;
	}
	squares|=((long long)1<<y)<<(x*CHAR_BIT);
	return 1;
}
//returns -1 if x or y are out of bounds
//and 1 if deoccupying the square is successful
int Bitboard::deoccupy(uint x, uint y){
	if((x|y)>7){
		return -1;
	}
	squares&=(((long long)1<<y)<<(x*CHAR_BIT)^(~(long long)0));
	return 1;
}
Bitboard* knightSet(){
	long long newSquares=0;
	Bitboard* newSet=new Bitboard(piece);
	int moveSquares[4]={6,10,15,17}
	for(int i=0;i<4;i++){
		newSquares|=squares<<moveSquares[i];
		newSquares|=squares>>moveSquares[i];
	}
	newSet->setSquares(newSquares);
	return newSet;
}
Bitboard* pawnSet(){

}
Bitboard* kingSet(){

}
Bitboard* rookSet(){

}
Bitboard* bishopSet(){

}
Bitboard* queenSet(){

}
Bitboard::~Bitboard(){
}