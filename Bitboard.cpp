
#include "Bitboard.h"
#define num_piece_types 12
#define numMoves 1024
const char chars[num_piece_types]={'P','R','N','B','Q','K','p','r','n','b','q','k'};
Bitboard::Bitboard(){
	bitbrds=new int64_t [num_piece_types];
	for(int i=0;i<num_piece_types;i++){
		bitbrds[i]=0;
	}
	startPosition();
	moveHistory=(move_t*)malloc(sizeof(move_t)*2048);
	plyNo=0;
}
int Bitboard::boardIndex(char c){
	int i=0;
	if(c>'Z')
		i=6;
	while(i<12){
		if(chars[i]==c){
			return i;
		}
		i++;
	}
	return -1;
}
//returns 1 if the square is occupied, 0 if not,
// and -1 if x or y are out of bounds
int Bitboard::isOccupied(uint x, uint y){
	if((x|y)>7){
		return -1;
	}
	//std::cout<< piece<<std::bitset<64> (bitbrds)<<"\n";
	int64_t compositeBoard=0;
	for(int i=0;i<num_piece_types;i++){
		compositeBoard|=((bitbrds[i]>>(x*CHAR_BIT))>>y);
	}
	return 1&compositeBoard;
}
//returns -1 if x or y are out of bounds
//and 1 if occupying the square is successful
int Bitboard::occupy(char piece,uint x,uint y){
	if((x|y)>7){
		return -1;
	}
	bitbrds[boardIndex(piece)]|=((int64_t)1<<y)<<(x*CHAR_BIT);
	return 1;
}
//returns -1 if x or y are out of bounds
//and 1 if deoccupying the square is successful
int Bitboard::deoccupy(uint x, uint y){
	if((x|y)>7){
		return -1;
	}for(int i=0;i<num_piece_types;i++){
		bitbrds[i]&=(((int64_t)1<<y)<<(x*CHAR_BIT)^(~(int64_t)0));
	}
	return 1;
}
int Bitboard::deoccupy(int index, uint x, uint y){
	if((x|y)>7){
		return -1;
	}
	bitbrds[index]&=(((int64_t)1<<y)<<(x*CHAR_BIT)^(~(int64_t)0));
	return 1;
}
//set up the start position
void Bitboard::startPosition(){
	for(int i=0;i<8;i++){
		occupy('P',i,1);
		occupy('p',i,6);
	}
	occupy('R',0,0);
	occupy('R',7,0);
	occupy('N',1,0);
	occupy('N',6,0);
	occupy('B',2,0);
	occupy('B',5,0);
	occupy('Q',3,0);
	occupy('K',4,0);
	occupy('r',0,7);
	occupy('r',7,7);
	occupy('n',1,7);
	occupy('n',6,7);
	occupy('b',2,7);
	occupy('b',5,7);
	occupy('q',3,7);
	occupy('k',4,7);
}
//return true if the move is legal, false otherwise
bool Bitboard::isLegal(move_t m){
	int64_t newBoard=bitbrds[m.pieceMoved]&((int64_t)1<<(m.x1*8+m.y1));
	switch(m.pieceMoved){
		case 0:
			return ((int64_t)1<<(m.x2*8+m.y1))&pawnSet(newBoard,0);
		case 1:
			return ((int64_t)1<<(m.x2*8+m.y1))&rookSet(newBoard,0);
		case 2:
			return ((int64_t)1<<(m.x2*8+m.y1))&knightSet(newBoard,0);
		case 3:
			return ((int64_t)1<<(m.x2*8+m.y1))&bishopSet(newBoard,0);
		case 4:
			return ((int64_t)1<<(m.x2*8+m.y1))&queenSet(newBoard,0);
		case 5:
			return ((int64_t)1<<(m.x2*8+m.y1))&kingSet(newBoard,0);
		case 6:
			return ((int64_t)1<<(m.x2*8+m.y1))&pawnSet(newBoard,1);
		case 7:
			return ((int64_t)1<<(m.x2*8+m.y1))&rookSet(newBoard,1);
		case 8:
			return ((int64_t)1<<(m.x2*8+m.y1))&knightSet(newBoard,1);
		case 9:
			return ((int64_t)1<<(m.x2*8+m.y1))&bishopSet(newBoard,1);
		case 10:
			return ((int64_t)1<<(m.x2*8+m.y1))&queenSet(newBoard,1);
		case 11:
			return ((int64_t)1<<(m.x2*8+m.y1))&kingSet(newBoard,1);
	}
	return false;
}
//make the move on the board, return true if successful
bool Bitboard::move(move_t m){
	bitbrds[m.pieceMoved]&=~(int64_t)0^((int64_t)1<<(m.x1*8+m.y1));
	bitbrds[m.pieceMoved]|=(int64_t)1<<(m.x2*8+m.y2);
	if(m.pieceTaken!=12){
		bitbrds[m.pieceTaken]&=~(int64_t)0^((int64_t)1<<(m.x2*8+m.y2));
	}
	moveHistory[plyNo]=m;
	plyNo++;
	return true;
}
//take back the previous move, return true if successful
bool Bitboard::takeBack(){
	//TODO
	return false;
}
//return the character representing the piece on the board.
//and if nothing is there return '_'
char Bitboard::piece(int x,int y){
	for(int i=0;i<num_piece_types;i++){
		if((bitbrds[i]>>(x*CHAR_BIT+y))&1){
			return chars[i];
		}
	}
	return '_';
}
//return a zero-terminated list of all moves that have been played in the game.
move_t* Bitboard::getMoveHistory(){
	//TODO
	return NULL;
}
int64_t Bitboard::knightSet(int64_t brd,int blackOrWhite){
	int64_t newSquares=0;
	int moveSquares[4]={6,10,15,17};
	for(int i=0;i<4;i++){
		newSquares|=brd<<moveSquares[i];
		newSquares|=brd>>moveSquares[i];
	}
	//this loop goes through all pieces of the same color as this piece
	//and turns off places where the knight would be taking its own piece.
	for(int i=myPieces(blackOrWhite);i<myPieces(blackOrWhite)+6;i++){
		newSquares&=(~bitbrds[i]);
	}
	return newSquares;
}
int64_t Bitboard::pawnSet(int64_t brd, int blackOrWhite){
	int64_t pushSquares=0;//the squares where the pawn would be pushing
	int64_t takeSquares=0;//the squares where the pawn would be taking
	int forward=1;
	if(blackOrWhite>5){
		forward=-1;
	}
	int64_t mask=(((int64_t)1)<<3);
	if(forward==-1)
		mask<<=1;
	for(int i=0;i<7;i++){
		mask<<=CHAR_BIT;
		mask|=1;
	}
	if(forward==1){
		pushSquares|=brd<<1;
		takeSquares|=brd<<(1+CHAR_BIT);
		takeSquares|=brd>>(CHAR_BIT-1);
		pushSquares|=(brd<<2)&mask;
	}
	else{
		pushSquares|=brd>>1;
		takeSquares|=brd>>(1+CHAR_BIT);
		takeSquares|=brd<<(CHAR_BIT-1);
		pushSquares|=(brd>>2)&mask;
	}
	for(int i=0;i<num_piece_types;i++){
		pushSquares&=(~bitbrds[i]);
	}
	for(int i=otherPieces(blackOrWhite);i<otherPieces(blackOrWhite)+6;i++){
		takeSquares&=(bitbrds[i]);
	}
	return pushSquares|takeSquares;
}
int64_t Bitboard::kingSet(int64_t brd, int blackOrWhite){
	int64_t newSquares=0;
	int moveSquares[4]={1,7,8,9};
	for(int i=0;i<4;i++){
		newSquares|=brd<<moveSquares[i];
		newSquares|=brd>>moveSquares[i];
	}
	for(int i=myPieces(blackOrWhite);i<myPieces(blackOrWhite)+6;i++){
		newSquares&=(~bitbrds[i]);
	}
	int64_t mask=((int64_t)1<<(2*(CHAR_BIT)))|((int64_t)1<<(6*(CHAR_BIT)));
	mask|=mask<<7;
	newSquares|=(brd>>(2*CHAR_BIT))&mask;
	return newSquares;
}
int64_t Bitboard::rookSet(int64_t brd, int blackOrWhite){
	int64_t newSquares=0;
	int directions[4]={1,8};
	int direction;
	for(int i=0;i<2;i++){
		direction=directions[i];
		//while(!isOccupied()){

		//}
	}
	return newSquares;
}
int64_t Bitboard::bishopSet(int64_t brd, int blackOrWhite){

}
int64_t Bitboard::queenSet(int64_t brd, int blackOrWhite){
	return rookSet(brd, blackOrWhite)|bishopSet(brd,blackOrWhite);
}
string Bitboard::tostring(){
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
Bitboard::~Bitboard(){
	free(moveHistory);
	delete [] bitbrds;
}
ostream& operator<<(ostream &strm, Bitboard* board){
	return strm << board->tostring();
}