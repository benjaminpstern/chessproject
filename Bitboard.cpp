
#include "Bitboard.h"
#include <iostream>
#include <bitset>
#define num_piece_types 12
#define numMoves 1024
const char chars[num_piece_types]={'P','R','N','B','Q','K','p','r','n','b','q','k'};
Bitboard::Bitboard(char c){
	bitbrds=new int64_t [num_piece_types];
	moveHistory=new move_t[1024];
}
int Bitboard::boardIndex(char c){
	int i=0;
	if(c>'Z')
		i=6;
	while(i<12){
		if(chars[i]==c){
			return i;
		}
		i++;
	}
	return -1;
}
//returns 1 if the square is occupied, 0 if not,
// and -1 if x or y are out of bounds
int Bitboard::isOccupied(uint x, uint y){
	if((x|y)>7){
		return -1;
	}
	//std::cout<< piece<<std::bitset<64> (bitbrds)<<"\n";
	int64_t compositeBoard=0;
	for(int i=0;i<num_piece_types;i++){
		compositeBoard|=((bitbrds[i]>>(x*CHAR_BIT))>>y)
	}
	return 1&compositeBoard;
}
//returns -1 if x or y are out of bounds
//and 1 if occupying the square is successful
int Bitboard::occupy(char piece,uint x,uint y){
	if((x|y)>7){
		return -1;
	}
	bitbrds[boardIndex(piece)]|=((int64_t)1<<y)<<(x*CHAR_BIT);
	return 1;
}
//returns -1 if x or y are out of bounds
//and 1 if deoccupying the square is successful
int Bitboard::deoccupy(uint x, uint y){
	if((x|y)>7){
		return -1;
	}for(int i=0;i<num_piece_types;i++){
		bitbrds[i]&=(((int64_t)1<<y)<<(x*CHAR_BIT)^(~(int64_t)0));
	}
	return 1;
}
int Bitboard::deoccupy(int index, uint x, uint y){
	if((x|y)>7){
		return -1;
	}
	bitbrds[index]&=(((int64_t)1<<y)<<(x*CHAR_BIT)^(~(int64_t)0));
	return 1;
}
int64_t Bitboard::knightSet(int index){
	int64_t newSquares=0;
	int moveSquares[4]={6,10,15,17};
	for(int i=0;i<4;i++){
		newSquares|=bitbrds[index]<<moveSquares[i];
		newSquares|=bitbrds[index]>>moveSquares[i];
	}
	//this loop goes through all pieces of the same color as this piece
	//and turns off places where the knight would be taking its own piece.
	for(i=(index/6)*6;i<(index/6)*6+6;i++){
		newSquares&=~bitbrds[index];
	}
	return newSquares;
}
int64_t Bitboard::pawnSet(int index){
	int64_t pushSquares=0;//the squares where the pawn would be pushing
	int64_t takeSquares=0;//the squares where the pawn would be taking
	int forward=1;
	if(index>5){
		forward=-1;
	}
	int64_t mask=(((int64_t)1)<<3);
	if(forward==-1)
		mask<<=1;
	for(int i=0;i<7;i++){
		mask<<=CHAR_BIT;
		mask|=1;
	}
	pushSquares|=bitbrds[index]<<forward;
	takeSquares|=bitbrds[index]<<(forward+CHAR_BIT);
	takeSquares|=bitbrds[index]<<(forward-CHAR_BIT);
	pushSquares|=(bitbrds[index]<<2*forward)&mask;
	for(int i=0;i<num_piece_types;i++){
		pushSquares&=(~bitbrds[index]);
	}
	for(int i=0;i<num_piece_types;i++){
		takeSquares&=(bitbrds[index]);
	}
	return pushSquares|takeSquares;
}
Bitboard* kingSet(int index){
	int64_t newSquares=0;
	int moveSquares[4]={1,7,8,9};
	for(int i=0;i<4;i++){
		newSquares|=bitbrds[index]<<moveSquares[i];
		newSquares|=bitbrds[index]>>moveSquares[i];
	}
	int64_t mask=((int64_t)1<<(2*(CHAR_BIT)))|((int64_t)1<<(6*(CHAR_BIT)))
	mask|=mask<<7;
	newSquares|=(bitbrds[index]>>(2*CHAR_BIT))&mask;
	return newSquares;
}
Bitboard* rookSet(){

}
Bitboard* bishopSet(){

}
Bitboard* queenSet(){

}
Bitboard::~Bitboard(){
}