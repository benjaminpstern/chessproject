#include "Bitboard.h"
#include <iostream>
using namespace std;
int main(){
	Bitboard* board=new Bitboard();
	//cout<< board;
	move_t m(0,0,3,3,'R','_');
	board->move(m);
	//cout<<board;
	//board->print_bitboard(board->ownPieces(1));
	board->print_bitboard(board->pieceSet(8));
	//board->print_bitboard(board->occupancySet());
	delete board;
	return 0;
}