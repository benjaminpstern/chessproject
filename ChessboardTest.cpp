#include "Bitboard.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
using namespace std;
int main(){
	srand(time(NULL));
	Bitboard* board=new Bitboard();
	//cout<< board;
	move_t m1(4,1,4,3,'P','_');
	move_t m2(5,6,5,4,'p','_');
	move_t m3(6,0,5,2,'N','_');
	/*for(int i=0;i<8;i++){
		for(int j=6;j<8;j++){
			board->deoccupy(i,j);
		}
	}*/
	/*cout<<board;
	board->move(m1);
	board->move(m2);
	board->move(m3);
	move_t* moves=board->allMoves();
	for(int i=0;moves[i]!=0;i++){
		board->move(moves[i]);
		cout<<board;
		board->takeBack();
	}
	delete [] moves;*/
	/*board->print_bitboard(board->pieceAttacks(3));
	board->move(m2);
	board->print_bitboard(board->pieceAttacks(5));
	board->move(m3);
	cout<<board;*/
	//board->print_bitboard(board->pieceAttacks(3));
	//board->print_bitboard(board->occupancySet());
	/*board->print_bitboard(board->pieceAttacks(5));
	board->takeBack();*/
	cout<<board;
	for(int i=0;i<2;i++){
		move_t* moves=board->nBestMoves(1);
		board->move(moves[0]);
		move_t m=moves[0];
		//printf("%d,%d to %d,%d a %c took a %c",m.getx1(),m.gety1(),m.getx2(), m.gety2(), m.getpieceMoved(),m.getpieceTaken());
		cout<<board;
		//delete [] moves;
	}
	
	//board->print_bitboard(board->occupancySet());
	//board->print_bitboard((0x8040201008040201>>8)|(0x8040201008040201<<1));
	/*uint64_t mask=1;
	mask<<=8*7;
	for(int i=0;i<7;i++){
		mask|=mask>>9;
	}
	printf("%llx\n",mask);*/
	delete board;
	return 0;
}