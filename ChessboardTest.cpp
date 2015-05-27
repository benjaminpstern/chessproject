#include "Bitboard.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
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
	for(int i=0;i<3;i++){
		vector<move_t>* movesPtr=board->nBestMoves(1,1);
		vector<move_t> moves=*movesPtr;
		cout<<"Size of moves is "<< moves.size()<<endl;
		if(moves[0]){
			board->move(moves[0]);
		}
		move_t m=moves[0];
		cout<<"moved"<<endl;
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
void testDraw3(){
	int whiteCount = 0;
	int blackCount = 0;
	int drawCount = 0;
	for(int i=0;i<100;i++){
		srand(time(NULL));
		Bitboard* board=new Bitboard();
		//cout<<board;
		int moveno=0;
		while(!board->isDraw()&&!board->isCheckmate()){
			vector<move_t>* movesPtr=board->allMoves();
			board->move((*movesPtr)[rand()%movesPtr->size()]);
			//cout<<board;
			//cout<<moveno<<endl;
			moveno++;
			//board->print_bitboard(board->pieceAttacks(8));
			delete movesPtr;
		}
		if(board->isDraw()){
			drawCount++;
		}
		if(board->isCheckmate()){
			if(board->isInCheck(0))
				blackCount++;
			else
				whiteCount++;
		}
		delete board;
	}
	cout<<"White wins: "<< whiteCount<<endl;
	cout<<"Black wins: "<< blackCount<<endl;
	cout<<"Draw: "<< drawCount<<endl;
}
void testQuiesce(){
	Bitboard* board=new Bitboard();
	cout<<board;
	for(int i=0;i<50;i++){
		vector<move_t>* movesPtr=board->nBestMoves(1,0);
		vector<move_t> moves=*movesPtr;
		if(moves[0]){
			board->move(moves[0]);
		}
		move_t m=moves[0];
		cout<<board;
		delete movesPtr;
	}
	delete board;
}
void testEvaluate(){
	Bitboard* board=new Bitboard();
	move_t m1(4,1,4,3,'P','_');
	move_t m2(0,6,0,5,'p','_');
	move_t m3(5,0,0,5,'B','p');
	move_t m4(0,7,0,5,'r','B');
	board->move(m1);
	board->move(m2);
	board->move(m3);
	board->move(m4);
	cout<<board;
	board->print_bitboard(board->pieceAttacks(9));
	cout<<board->evaluate()<<endl;
	for(int i=0;i<4;i++){
		board->takeBack();
	}
	cout<<board;
	delete board;
}
void playTest(){
	Bitboard* board=new Bitboard();
	cout<<board;
	int moveno=0;
	while(!board->isDraw()&&!board->isCheckmate()){
		vector<move_t>* movesPtr=board->nBestMoves(1,1);
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
void playTest2(){
	time_t timePerMove = 5;
	Bitboard* board=new Bitboard();
	cout<<board;
	int moveno=0;
	while(!board->isDraw()&&!board->isCheckmate()){
		time_t curTime;
		time_t timeBeforeMove = time(NULL);
		int depth = 0;
		move_t m;
		while((curTime = time(NULL)) < timeBeforeMove + timePerMove){
			vector<move_t>* movesPtr=board->nBestMoves(1,depth);
			m = (*movesPtr)[0];
			//cout<<moveno<<endl;
			moveno++;
			//board->print_bitboard(board->pieceAttacks(8));
			delete movesPtr;
			depth++;
		}
		board->move(m);
		cout<<board;
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
move_t moveFromInput(string s, Bitboard* board){
	move_t nullMove = 0;
	if(s.size() < 5){
		return nullMove;
	}
	uint x1 = s[0] - 'a';
	uint y1 = s[1] - '1';
	uint x2 = s[3] - 'a';
	uint y2 = s[4] - '1';
	if(x1 > 7 || y1 > 7 || x2 > 7 || y2 > 7){
		return nullMove;
	}
	move_t m = move_t(x1,y1,x2,y2,board->piece(x1,y1),board->piece(x2,y2));
	return m;
}
void playTest3(){
	time_t timePerMove = 5;
	Bitboard* board=new Bitboard();
	cout<<board;
	int moveno=0;
	while(!board->isDraw()&&!board->isCheckmate()){
		time_t curTime;
		time_t timeBeforeMove = time(NULL);
		int depth = 2;
		move_t m;
		while((curTime = time(NULL)) < timeBeforeMove + timePerMove){
			cout<<depth<<endl;
			vector<move_t>* movesPtr=board->nBestMoves(1,depth);
			m = (*movesPtr)[0];
			//cout<<moveno<<endl;
			moveno++;
			//board->print_bitboard(board->pieceAttacks(8));
			delete movesPtr;
			depth++;
		}
		board->move(m);
		cout<<board;
		string playerInput;
		move_t playerMove = 0;
		while(!board->isLegal(playerMove)){
			cin >> playerInput;
			playerMove = moveFromInput(playerInput,board);
		}
		board->move(playerMove);
		cout<<board;
		
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
void bugFix1(){
	time_t timePerMove = 5;
	Bitboard* board=new Bitboard();
	string moveList[] = {"d2-d4","d7-d5","g1-f3","g8-f6","b1-c3","c8-f5",
		"c1-f4","c7-c6","e2-e3","h7-h6","f1-e2","e7-e6","a2-a4","b8-d7",
		"h2-h4","f8-b4","a4-a5","b4-a5","e2-d3","f6-e4","d3-e4","f5-e4",
		"f4-d6","d7-f6","b2-b4","d8-d6","b4-a5","e4-g6","f3-e5","g6-h7",
		"a5-a6","b7-b6","h4-h5","e8-g8","d1-g4"};
	for(int i=0;i<35;i++){
		board->move(moveFromInput(moveList[i],board));
	}
	cout<<board;
	//board->print_bitboard(board->squaresToKing(1));
	std::vector<move_t>* moves = board->allMoves();
	for(int i=0;i<moves->size();i++){
		cout<<(*moves)[i].tostring()<<endl;
	}
	board->print_bitboard(board->pieceAttacks(4));
	board->print_bitboard(board->filePins(1));
	delete moves;
	/*int moveno=0;
	while(!board->isDraw()&&!board->isCheckmate()){
		time_t curTime;
		time_t timeBeforeMove = time(NULL);
		int depth = 0;
		move_t m;
		while((curTime = time(NULL)) < timeBeforeMove + timePerMove){
			vector<move_t>* movesPtr=board->nBestMoves(1,depth);
			m = (*movesPtr)[0];
			//cout<<moveno<<endl;
			moveno++;
			//board->print_bitboard(board->pieceAttacks(8));
			delete movesPtr;
			depth++;
		}
		board->move(m);
		cout<<board;
		string playerInput;
		move_t playerMove = 0;
		while(!board->isLegal(playerMove)){
			cin >> playerInput;
			playerMove = moveFromInput(playerInput,board);
		}
		board->move(playerMove);
		cout<<board;
		
	}
	if(board->isDraw()){
		cout<<"DRAW"<<endl;
	}
	if(board->isCheckmate()){
		if(board->isInCheck(0))
			cout<<"BLACK WINS"<<endl;
		else
			cout<<"WHITE WINS"<<endl;
	}*/
	delete board;
}
void bugFix2(){
	time_t timePerMove = 5;
	Bitboard* board=new Bitboard();
	string moveList[] = {"d2-d4","d7-d5","g1-f3","g8-f6","b1-c3","c8-f5",
		"c1-f4","c7-c6","e2-e3","h7-h6","f1-e2","e7-e6","a2-a4","b8-d7",
		"h2-h4","f8-b4","a4-a5","b4-a5","e2-d3","f6-e4","d3-e4","f5-e4",
		"f4-d6","d7-f6","b2-b4","d8-d6","b4-a5","e4-g6","f3-e5","g6-h7",
		"a5-a6","b7-b6","h4-h5","e8-g8","h1-h4","f8-c8"};
	for(int i=0;i<36;i++){
		board->move(moveFromInput(moveList[i],board));
	}
	cout<<"cancastle" << board->whiteKCastle<<endl;
	cout<<board;
	//board->print_bitboard(board->squaresToKing(1));
	std::vector<move_t>* moves = board->allMoves();
	for(int i=0;i<moves->size();i++){
		cout<<(*moves)[i].tostring()<<endl;
	}
	board->print_bitboard(board->pieceAttacks(4));
	board->print_bitboard(board->filePins(1));
	delete moves;
	delete board;
}
void testGame1(){
	time_t timePerMove = 5;
	Bitboard* board=new Bitboard();
	string moveList[] = {"d2-d4","d7-d5","g1-f3","g8-f6","b1-c3","c8-f5",
		"c1-f4","c7-c6","e2-e3","h7-h6","f1-e2","e7-e6","a2-a4","b8-d7",
		"h2-h4","f8-b4","a4-a5","b4-a5","e2-d3","f6-e4","d3-e4","f5-e4",
		"f4-d6","d7-f6","b2-b4","d8-d6","b4-a5","e4-g6","f3-e5","g6-h7",
		"a5-a6","b7-b6","h4-h5","e8-g8"};
	for(int i=0;i<34;i++){
		board->move(moveFromInput(moveList[i],board));
	}
	cout<<board;
	int moveno=0;
	while(!board->isDraw()&&!board->isCheckmate()){
		time_t curTime;
		time_t timeBeforeMove = time(NULL);
		int depth = 0;
		move_t m;
		while((curTime = time(NULL)) < timeBeforeMove + timePerMove){
			vector<move_t>* movesPtr=board->nBestMoves(1,depth);
			m = (*movesPtr)[0];
			//cout<<moveno<<endl;
			moveno++;
			//board->print_bitboard(board->pieceAttacks(8));
			delete movesPtr;
			depth++;
		}
		board->move(m);
		cout<<board;
		string playerInput;
		move_t playerMove = 0;
		while(!board->isLegal(playerMove)){
			cin >> playerInput;
			playerMove = moveFromInput(playerInput,board);
		}
		board->move(playerMove);
		cout<<board;
		
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
void testEndgame(){
	time_t timePerMove = 5;
	Bitboard* board=new Bitboard();
	/*string moveList[] = {"d2-d4","d7-d5","g1-f3","g8-f6","b1-c3","c8-f5",
		"c1-f4","c7-c6","e2-e3","h7-h6","f1-e2","e7-e6","a2-a4","b8-d7",
		"h2-h4","f8-b4","a4-a5","b4-a5","e2-d3","f6-e4","d3-e4","f5-e4",
		"f4-d6","d7-f6","b2-b4","d8-d6","b4-a5","e4-g6","f3-e5","g6-h7",
		"a5-a6","b7-b6","h4-h5","e8-g8"};
	for(int i=0;i<34;i++){
		board->move(moveFromInput(moveList[i],board));
	}*/
	for(int i=0;i<12;i++){
		if(i == 4||i==5||i==11){
			continue;
		}
		board->bitbrds[i] = 0;
	}
	board->whiteKCastle = -2;
	board->whiteQCastle = -2;
	board->blackKCastle = -2;
	board->blackQCastle = -2;
	cout<<board;
	int moveno=0;
	while(!board->isDraw()&&!board->isCheckmate()){
		time_t curTime;
		time_t timeBeforeMove = time(NULL);
		int depth = 0;
		move_t m;
		while((curTime = time(NULL)) < timeBeforeMove + timePerMove){
			cout<<depth<<endl;
			vector<move_t>* movesPtr=board->nBestMoves(1,depth);
			m = (*movesPtr)[0];
			//cout<<moveno<<endl;
			moveno++;
			//board->print_bitboard(board->pieceAttacks(8));
			delete movesPtr;
			depth++;
		}
		board->move(m);
		cout<<board;
		string playerInput;
		move_t playerMove = 0;
		while(!board->isLegal(playerMove)){
			cin >> playerInput;
			playerMove = moveFromInput(playerInput,board);
		}
		board->move(playerMove);
		cout<<board;
		
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
void testEndgame2(){
	time_t timePerMove = 5;
	Bitboard* board=new Bitboard();
	for(int i=0;i<12;i++){
		if(i == 1||i==5||i==11){
			continue;
		}
		board->bitbrds[i] = 0;
	}
	board->bitbrds[1] = 1;
	board->whiteKCastle = -2;
	board->whiteQCastle = -2;
	board->blackKCastle = -2;
	board->blackQCastle = -2;
	/*string moveList[] = {"a1-b1","e8-f8","b1-a1","f8-e8","a1-b1",
	"e8-f8","b1-a1","f8-e8","a1-b1","e8-f8","b1-a1","f8-e8","a1-b1",
	"e8-f8","b1-a1","f8-e8","a1-b1","e8-f8","b1-a1","f8-e8","a1-b1","e8-f8","b1-a1",
	"f8-e8","a1-b1","e8-f8","b1-a1","f8-e8","a1-b1","e8-f8","b1-a1","f8-e8","a1-b1",
	"e8-f8","b1-a1","f8-e8","a1-b1","e8-f8","b1-a1","f8-e8"};
	for(int i=0;i<40;i++){
		board->move(moveFromInput(moveList[i],board));
	}*/
	cout<<board;
	int moveno=0;
	while(!board->isDraw()&&!board->isCheckmate()){
		time_t curTime;
		time_t timeBeforeMove = time(NULL);
		int depth = 0;
		move_t m;
		while((curTime = time(NULL)) < timeBeforeMove + timePerMove){
			vector<move_t>* movesPtr=board->nBestMoves(1,depth);
			m = (*movesPtr)[0];
			//cout<<moveno<<endl;
			moveno++;
			//board->print_bitboard(board->pieceAttacks(8));
			delete movesPtr;
			depth++;
		}
		board->move(m);
		cout<<board;
		string playerInput;
		move_t playerMove = 0;
		while(!board->isLegal(playerMove)){
			cin >> playerInput;
			playerMove = moveFromInput(playerInput,board);
		}
		board->move(playerMove);
		cout<<board;
		
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
	testGame1();
	return 0;
}
