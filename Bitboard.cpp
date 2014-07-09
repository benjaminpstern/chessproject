
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
//returns 0 if index is between 0
//and 5 and if index is between 6 and 11
//returns 6
int myPieces(int index){
	return (index/6)*6;
}
int otherPieces(int index){
	return myPieces((index+6)%12);
}
int64_t knightSet(int64_t brd,int blackOrWhite){
	int64_t newSquares=0;
	int moveSquares[4]={6,10,15,17};
	for(int i=0;i<4;i++){
		newSquares|=brd<<moveSquares[i];
		newSquares|=brd>>moveSquares[i];
	}
	//this loop goes through all pieces of the same color as this piece
	//and turns off places where the knight would be taking its own piece.
	for(i=myPieces(blackOrWhite);i<myPieces(blackOrWhite)+6;i++){
		newSquares&=~bitbrds[i];
	}
	return newSquares;
}
int64_t pawnSet(int64_t brd, int blackOrWhite){
	int64_t pushSquares=0;//the squares where the pawn would be pushing
	int64_t takeSquares=0;//the squares where the pawn would be taking
	int forward=1;
	if(blackOrWhite>5){
		forward=-1;
	}
	int64_t mask=(((int64_t)1)<<3);
	if(forward==-1)
		mask<<=1;
	for(int i=0;i<7;i++){
		mask<<=CHAR_BIT;
		mask|=1;
	}
	if(forward==1){
		pushSquares|=brd<<1;
		takeSquares|=brd<<(1+CHAR_BIT);
		takeSquares|=brd>>(CHAR_BIT-1);
		pushSquares|=(brd<<2)&mask;
	}
	else{
		pushSquares|=brd>>1;
		takeSquares|=brd>>(1+CHAR_BIT);
		takeSquares|=brd<<(CHAR_BIT-1);
		pushSquares|=(brd>>2)&mask;
	}
	for(int i=0;i<num_piece_types;i++){
		pushSquares&=(~bitbrds[i]);
	}
	for(int i=otherPieces(blackOrWhite);i<otherPieces(blackOrWhite)+6;i++){
		takeSquares&=(bitbrds[i]);
	}
	return pushSquares|takeSquares;
}
Bitboard* kingSet(int64_t brd, int blackOrWhite){
	int64_t newSquares=0;
	int moveSquares[4]={1,7,8,9};
	for(int i=0;i<4;i++){
		newSquares|=brd<<moveSquares[i];
		newSquares|=brd>>moveSquares[i];
	}
	for(int i=myPieces(blackOrWhite);i<myPieces(blackOrWhite)+6;i++){
		newSquares&=(~bitbrds[i]);
	}
	int64_t mask=((int64_t)1<<(2*(CHAR_BIT)))|((int64_t)1<<(6*(CHAR_BIT)))
	mask|=mask<<7;
	newSquares|=(brd>>(2*CHAR_BIT))&mask;
	return newSquares;
}
Bitboard* rookSet(int64_t brd, int blackOrWhite){
	int64_t newSquares=0;
	int directions[4]={1,8};
	int direction;
	for(int i=0;i<2;i++){
		direction=directions[i];
		while(!isOccupied())
	}
}
Bitboard* bishopSet(int brd, int blackOrWhite){

}
Bitboard* queenSet(int brd, int blackOrWhite){
	return rookSet(brd, blackOrWhite)|bishopSet(brd,blackOrWhite);
}
Bitboard::~Bitboard(){
}