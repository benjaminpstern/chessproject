#include "Bitboard.h"
#include <iostream>
#include <stdio.h>
using namespace std;
int main(){
	Bitboard* board=new Bitboard();
	//cout<< board;
	move_t m(4,1,4,3,'P','_');
	/*for(int i=0;i<8;i++){
		for(int j=6;j<8;j++){
			board->deoccupy(i,j);
		}
	}*/
	board->move(m);
	cout<<board;
	//board->print_bitboard(board->pieceAttacks(3));
	//board->print_bitboard(board->occupancySet());
	board->print_bitboard(board->pieceAttacks(4));
	board->takeBack();
	
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