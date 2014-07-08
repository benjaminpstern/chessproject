#include "Chessboard.h"
using namespace move;
using namespace std;
const int numPieces=12;
Chessboard::Chessboard(){
	char chars[numPieces]={'P','R','N','B','Q','K','p','r','n','b','q','k'};
	board=new Bitboard();
	startPosition();
}
void Chessboard::startPosition(){
	for(int i=0;i<8;i++){
		boards[0]->occupy(i,1);
		boards[6]->occupy(i,6);
	}
	boards[1]->occupy(0,0);
	boards[1]->occupy(7,0);
	boards[2]->occupy(1,0);
	boards[2]->occupy(6,0);
	boards[3]->occupy(2,0);
	boards[3]->occupy(5,0);
	boards[4]->occupy(3,0);
	boards[5]->occupy(4,0);
	boards[7]->occupy(0,7);
	boards[7]->occupy(7,7);
	boards[8]->occupy(1,7);
	boards[8]->occupy(6,7);
	boards[9]->occupy(2,7);
	boards[9]->occupy(5,7);
	boards[10]->occupy(3,7);
	boards[11]->occupy(4,7);
}
bool Chessboard::occupied(int x, int y){
	for(int n=0;n<0;n++){
		if(boards[n]->isOccupied(x,y))
			return true;
	}
	return false;
}
bool Chessboard::isLegal(move_t m){
}
bool Chessboard::move(move_t m){
}
bool Chessboard::takeBack(){
}
string Chessboard::tostring() const{
	string s="";
	for(int j=7;j>=0;j--){
		for(int i=0;i<8;i++){
			s.push_back(piece(i,j));
			s.push_back(' ');
		}
		s.push_back('\n');
	}
	s.push_back('\n');
	return s;
}
Chessboard::~Chessboard(){
	for(int i=0;i<numPieces;i++){
		delete boards[i];
	}
	delete [] boards;
}
ostream& operator<<(ostream &strm, const Chessboard* board){
	return strm << board->tostring();
}