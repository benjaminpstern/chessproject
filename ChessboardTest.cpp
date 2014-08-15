#include "Bitboard.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
using namespace std;
void testMove1(){
	Bitboard* board=new Bitboard();
	move_t m1(4,1,4,3,'P','_');
	move_t m2(5,6,5,4,'p','_');
	move_t m3(6,0,5,2,'N','_');
	board->move(m1);
	board->move(m2);
	board->move(m3);
	cout<<board;
	board->print_bitboard(board->pieceAttacks(4));
	for(int i=0;i<3;i++){
		board->takeBack();
	}
	cout<<board;
	delete board;
}
void testBestMoves(){
	Bitboard* board=new Bitboard();
	cout<<board;
	for(int i=0;i<20;i++){
		move_t* moves=board->nBestMoves(1);
		board->move(moves[0]);
		move_t m=moves[0];
		cout<<board;
	}
	delete board;
}
void testBestMoves1(){
	Bitboard* board=new Bitboard();
	cout<<board;
	move_t* moves=board->allMoves();
	for(int i=0;moves[i];i++){
		board->move(moves[i]);
		cout<<board;
		board->takeBack();
	}
	delete board;
}
int main(){
	testBestMoves();
	return 0;
}