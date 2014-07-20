#include "Bitboard.h"
#include <iostream>
using namespace std;
int main(){
	Bitboard* board=new Bitboard();
	//cout<< board;
	move_t m(0,0,2,3,'R','_');
	/*for(int i=0;i<8;i++){
		for(int j=6;j<8;j++){
			board->deoccupy(i,j);
		}
	}*/
	board->move(m);
	//cout<<board;
	board->print_bitboard(board->pieceAttacks(1));
	board->print_bitboard(board->occupancySet());
	board->takeBack();
	board->print_bitboard(board->pieceAttacks(1));
	board->print_bitboard(board->occupancySet());
	//board->print_bitboard(board->occupancySet());
	delete board;
	return 0;
}