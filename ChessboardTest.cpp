#include "Chessboard.h"
#include <iostream>
using namespace std;
int main(){
	Chessboard* board=new Chessboard();
	cout<< board;
	delete board;
	return 0;
}