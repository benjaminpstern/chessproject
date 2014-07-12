#include "Bitboard.h"
#include <iostream>
using namespace std;
int main(){
	Bitboard* board=new Bitboard();
	cout<< board;
	move_t m(1,1,1,3,'P','_');
	board->move(m);
	cout<<board;
	delete board;
	return 0;
}