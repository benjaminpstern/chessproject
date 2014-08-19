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
		vector<move_t>* movesPtr=board->nBestMoves(1);
		vector<move_t> moves=*movesPtr;
		board->move(moves[0]);
		move_t m=moves[0];
		cout<<board;
		delete movesPtr;
	}
	delete board;
}
void testBestMoves1(){
	Bitboard* board=new Bitboard();
	cout<<board;
	vector<move_t>* movesPtr=board->allMoves();
	vector<move_t> moves=*movesPtr;
	for(int i=0;i<moves.size();i++){
		board->move(moves[i]);
		cout<<board;
		board->takeBack();
	}
	delete movesPtr;
	delete board;
}
void testCheck(){
	Bitboard* board=new Bitboard();
	cout<<board;
	move_t m1(4,1,4,3,'P','_');
	move_t m2(3,6,3,4,'p','_');
	move_t m3(5,0,1,4,'B','_');
	move_t m4(2,7,3,6,'b','_');
	move_t m5(1,0,2,2,'N','_');
	board->move(m1);
	board->move(m2);
	board->move(m3);
	board->move(m4);
	board->move(m5);
	cout<<board;
	board->print_bitboard(board->pieceAttacks(9));
	cout<<board->isInCheck(1)<<endl;
	vector<move_t>* movesPtr=board->allMoves();
	vector<move_t> moves=*movesPtr;
	for(int i=0;i<moves.size();i++){
		board->move(moves[i]);
		cout<<board;
		board->takeBack();
	}
	delete movesPtr;
	delete board;
}
void testCheck2(){
	Bitboard* board=new Bitboard();
	cout<<board;
	move_t moveList[10];
	moveList[0]=move_t(4,1,4,3,'P','_');
	moveList[1]=move_t(5,6,5,5,'p','_');
	moveList[2]=move_t(3,1,3,3,'P','_');
	moveList[3]=move_t(6,6,6,4,'p','_');
	moveList[4]=move_t(3,0,7,4,'Q','_');
	for(int i=0;i<5;i++){
		board->move(moveList[i]);
	}
	cout<<board;
	board->print_bitboard(board->pieceAttacks(4));
	cout<<board->isCheckmate()<<endl;
	vector<move_t>* movesPtr=board->allMoves();
	vector<move_t> moves=*movesPtr;
	for(int i=0;i<moves.size();i++){
		board->move(moves[i]);
		cout<<board;
		board->takeBack();
	}
	delete movesPtr;
	delete board;
}
void testCheck3(){
	Bitboard* board=new Bitboard();
	cout<<board;
	move_t moveList[11];
	moveList[0]=move_t(4,1,4,3,'P','_');
	moveList[1]=move_t(2,6,2,5,'p','_');
	moveList[2]=move_t(3,1,3,3,'P','_');
	moveList[3]=move_t(3,6,3,4,'p','_');
	moveList[4]=move_t(1,0,2,2,'N','_');
	moveList[5]=move_t(3,4,4,3,'p','P');
	moveList[6]=move_t(2,2,4,3,'N','p');
	moveList[7]=move_t(1,7,3,6,'n','_');
	moveList[8]=move_t(3,0,4,1,'Q','_');
	moveList[9]=move_t(6,7,5,5,'n','_');
	moveList[10]=move_t(4,3,3,5,'N','_');
	for(int i=0;i<11;i++){
		board->move(moveList[i]);
	}
	cout<<board;
	board->print_bitboard(board->pieceAttacks(4));
	cout<<board->isCheckmate()<<endl;
	vector<move_t>* movesPtr=board->allMoves();
	vector<move_t> moves=*movesPtr;
	for(int i=0;i<moves.size();i++){
		board->move(moves[i]);
		cout<<board;
		board->takeBack();
	}
	delete movesPtr;
	delete board;
}
void testCheck4(){
	Bitboard* board=new Bitboard();
	cout<<board;
	move_t moveList[11];
	moveList[0]=move_t(4,1,4,3,'P','_');
	moveList[1]=move_t(2,6,2,5,'p','_');
	moveList[2]=move_t(3,1,3,3,'P','_');
	moveList[3]=move_t(3,6,3,4,'p','_');
	moveList[4]=move_t(1,0,2,2,'N','_');
	moveList[5]=move_t(3,4,4,3,'p','P');
	moveList[6]=move_t(2,2,4,3,'N','p');
	moveList[7]=move_t(1,7,3,6,'n','_');
	moveList[8]=move_t(3,0,4,1,'Q','_');
	moveList[9]=move_t(4,6,4,5,'p','_');
	moveList[10]=move_t(4,3,3,5,'N','_');
	for(int i=0;i<11;i++){
		board->move(moveList[i]);
	}
	cout<<board;
	board->print_bitboard(board->pieceAttacks(4));
	cout<<board->isCheckmate()<<endl;
	vector<move_t>* movesPtr=board->allMoves();
	vector<move_t> moves=*movesPtr;
	for(int i=0;i<moves.size();i++){
		board->move(moves[i]);
		cout<<board;
		board->takeBack();
	}
	delete movesPtr;
	delete board;
}
void testCheck5(){
	Bitboard* board=new Bitboard();
	cout<<board;
	move_t m1(4,1,4,3,'P','_');
	move_t m2(3,6,3,4,'p','_');
	move_t m3(5,0,1,4,'B','_');
	move_t m4(2,7,3,6,'b','_');
	move_t m5(1,0,2,2,'N','_');
	board->move(m1);
	board->move(m2);
	/*board->move(m3);
	board->move(m4);
	board->move(m5);*/
	cout<<board;
	board->print_bitboard(board->pieceAttacks(9));
	cout<<board->isInCheck(1)<<endl;
	vector<move_t>* movesPtr=board->allMoves();
	vector<move_t> moves=*movesPtr;
	for(int i=0;i<moves.size();i++){
		//cout<<moves[i].tostring();
		board->move(moves[i]);
		cout<<board;
		board->takeBack();
	}
	delete movesPtr;
	delete board;
}
void testDraw(){
	Bitboard* board=new Bitboard();
	cout<<board;
	int moveno=0;
	while(!board->isDraw()&&!board->isCheckmate()){
		vector<move_t>* movesPtr=board->allMoves();
		board->move((*movesPtr)[0]);
		cout<<board;
		//cout<<moveno<<endl;
		moveno++;
		//board->print_bitboard(board->pieceAttacks(8));
		delete movesPtr;
	}
	if(board->isDraw()){
		cout<<"DRAW"<<endl;
	}
	if(board->isCheckmate()){
		if(board->isInCheck(0))
			cout<<"BLACK WINS"<<endl;
		else
			cout<<"WHITE WINS"<<endl;
	}
	delete board;

}
void testDraw2(){
	srand(time(NULL));
	Bitboard* board=new Bitboard();
	cout<<board;
	int moveno=0;
	while(!board->isDraw()&&!board->isCheckmate()){
		vector<move_t>* movesPtr=board->allMoves();
		board->move((*movesPtr)[rand()%movesPtr->size()]);
		cout<<board;
		//cout<<moveno<<endl;
		moveno++;
		//board->print_bitboard(board->pieceAttacks(8));
		delete movesPtr;
	}
	if(board->isDraw()){
		cout<<"DRAW"<<endl;
	}
	if(board->isCheckmate()){
		if(board->isInCheck(0))
			cout<<"BLACK WINS"<<endl;
		else
			cout<<"WHITE WINS"<<endl;
	}
	delete board;

}
int main(){
	testCheck5();
	return 0;
}