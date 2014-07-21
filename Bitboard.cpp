
#include "Bitboard.h"
#define num_piece_types 12
#define numMoves 1024
const char chars[num_piece_types]={'P','R','N','B','Q','K','p','r','n','b','q','k'};
const int pieceValues[num_piece_types]={1,5,3,3,9,INT_MAX,-1,-5,-3,-3,-9,-INT_MAX};
const move_t nullMove;
Bitboard::Bitboard(){
	bitbrds=new uint64_t [num_piece_types];
	for(int i=0;i<num_piece_types;i++){
		bitbrds[i]=0;
	}
	startPosition();
	moveHistory=(move_t*)malloc(sizeof(move_t)*2048);
	plyNo=0;
	moveHistory[plyNo]=nullMove;
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
	uint64_t compositeBoard=0;
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
	bitbrds[boardIndex(piece)]|=((uint64_t)1<<y)<<(x*CHAR_BIT);
	return 1;
}
//returns -1 if x or y are out of bounds
//and 1 if deoccupying the square is successful
int Bitboard::deoccupy(uint x, uint y){
	if((x|y)>7){
		return -1;
	}for(int i=0;i<num_piece_types;i++){
		bitbrds[i]&=(((uint64_t)1<<y)<<(x*CHAR_BIT)^(~(uint64_t)0));
	}
	return 1;
}
int Bitboard::deoccupy(int index, uint x, uint y){
	if((x|y)>7){
		return -1;
	}
	bitbrds[index]&=(((uint64_t)1<<y)<<(x*CHAR_BIT)^(~(uint64_t)0));
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
uint64_t Bitboard::occupancySet(){
	uint64_t occ=0;
	for(int i=0;i<12;i++){
		occ|=bitbrds[i];
	}
	return occ;
}
uint64_t Bitboard::ownPieces(int blackOrWhite){
	uint64_t set=0;
	for(int i=myPieces(blackOrWhite);i<myPieces(blackOrWhite)+6;i++){
		//cout<<i<<endl;
		//print_bitboard(bitbrds[i]);
		set|=(bitbrds[i]);
	}
	return set;
}
uint64_t Bitboard::enemyPieces(int blackOrWhite){
	uint64_t set=0;
	for(int i=otherPieces(blackOrWhite);i<otherPieces(blackOrWhite)+6;i++){
		set|=(bitbrds[i]);
	}
	return set;
}
uint64_t Bitboard::pieceAttacks(int pieceIndex){
	switch(pieceIndex){
		case 0:
			return pawnAttacks(bitbrds[pieceIndex],0);
		case 1:
			return rookAttacks(bitbrds[pieceIndex],0);
		case 2:
			return knightAttacks(bitbrds[pieceIndex],0);
		case 3:
			return bishopAttacks(bitbrds[pieceIndex],0);
		case 4:
			return queenAttacks(bitbrds[pieceIndex],0);
		case 5:
			return kingAttacks(bitbrds[pieceIndex],0);
		case 6:
			return pawnAttacks(bitbrds[pieceIndex],1);
		case 7:
			return rookAttacks(bitbrds[pieceIndex],1);
		case 8:
			return knightAttacks(bitbrds[pieceIndex],1);
		case 9:
			return bishopAttacks(bitbrds[pieceIndex],1);
		case 10:
			return queenAttacks(bitbrds[pieceIndex],1);
		case 11:
			return kingAttacks(bitbrds[pieceIndex],1);
	}
}
//return true if the move is legal, false otherwise
bool Bitboard::isLegal(move_t m){
	uint64_t newBoard=bitbrds[m.pieceMoved]&((uint64_t)1<<(m.x1*8+m.y1));
	return ((uint64_t)1<<(m.x2*8+m.y1))&pieceAttacks(m.pieceMoved);
}
//make the move on the board, return true if successful
bool Bitboard::move(move_t m){
	bitbrds[m.pieceMoved]&=~((uint64_t)1<<(m.x1*8+m.y1));//turn off the bit that the piece moved from
	bitbrds[m.pieceMoved]|=((uint64_t)1<<(m.x2*8+m.y2));//turn on the place that the piece moved to
	if(m.pieceTaken!=12){
		bitbrds[m.pieceTaken]&=~(uint64_t)0^((uint64_t)1<<(m.x2*8+m.y2));//turn off the place where the piece got taken
	}
	moveHistory[plyNo]=m;
	plyNo++;
	moveHistory[plyNo]=nullMove;
	return true;
}
//take back the previous move, return true if successful
bool Bitboard::takeBack(){
	plyNo--;
	move_t m=moveHistory[plyNo];
	bitbrds[m.pieceMoved] |= ((uint64_t)1<<(m.x1*8+m.y1));
	bitbrds[m.pieceMoved] &= ~((uint64_t)1<<(m.x2*8+m.y2));
	if(m.pieceTaken!=12){
		bitbrds[m.pieceTaken]|=(uint64_t)0^((uint64_t)1<<(m.x2*8+m.y2));//turn on the place where the piece got taken
	}
	moveHistory[plyNo]=nullMove;
	return true;
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
	return moveHistory;
}
uint64_t Bitboard::knightAttacks(uint64_t brd,int blackOrWhite){
	uint64_t newSquares=0;
	newSquares|=(brd>>6) & notFirstSecondRank;
	newSquares|=(brd<<6) & notTwoLastRanks;
	newSquares|=(brd<<10)& notFirstSecondRank;
	newSquares|=(brd>>10)& notTwoLastRanks;
	newSquares|=(brd>>15)& notFirstRank;
	newSquares|=(brd<<15)& notLastRank;
	newSquares|=(brd<<17)& notFirstRank;
	newSquares|=(brd>>17)& notLastRank;
	//newSquares&=(~ownPieces(blackOrWhite));
	return newSquares;
}
uint64_t Bitboard::pawnAttacks(uint64_t brd, int blackOrWhite){
	uint64_t pushSquares=0;//the squares where the pawn would be pushing
	uint64_t takeSquares=0;//the squares where the pawn would be taking
	int forward=1;
	if(blackOrWhite>0){
		forward=-1;
	}
	uint64_t mask=rank(3);
	if(forward==-1)
		mask<<=1;
	if(forward==1){
		pushSquares|=brd<<1;
		takeSquares|=brd<<(1+CHAR_BIT);
		takeSquares|=brd>>(CHAR_BIT-1);
		uint64_t doublePush=(brd<<2)&mask;
		uint64_t occ=occupancySet();
		pushSquares|=(doublePush&(~occ&~(occ<<1)));
		//print_bitboard((~occ&~(occ<<1)));
	}
	else{
		pushSquares|=brd>>1;
		takeSquares|=brd>>(1+CHAR_BIT);
		takeSquares|=brd<<(CHAR_BIT-1);
		uint64_t doublePush=(brd>>2)&mask;
		uint64_t occ=occupancySet();
		pushSquares|=(doublePush&(~occ&~(occ>>1)));
	}
	pushSquares &=(~occupancySet());
	takeSquares &= occupancySet();
	return pushSquares|takeSquares;
}
uint64_t Bitboard::kingAttacks(uint64_t brd, int blackOrWhite){
	uint64_t newSquares=0;
	uint64_t taboo=0;
	int moveSquares[4]={1,7,8,9};
	for(int i=0;i<4;i++){
		newSquares|=brd<<moveSquares[i];
		newSquares|=brd>>moveSquares[i];
	}
	//newSquares&=~(ownPieces(blackOrWhite));
	for(int i=otherPieces(blackOrWhite);i<otherPieces(blackOrWhite)+6;i++){
		taboo|=pieceAttacks(i);
	}
	uint64_t mask=((uint64_t)1<<(2*(CHAR_BIT)))|((uint64_t)1<<(6*(CHAR_BIT)));
	mask|=mask<<7;
	newSquares|=(brd>>(2*CHAR_BIT))&mask;
	newSquares&=~taboo;
	return newSquares;
}
uint64_t Bitboard::rookAttacks(uint64_t brd, int blackOrWhite){
	uint64_t newSquares=0;
	uint64_t occ=occupancySet();
	uint64_t rooks=brd;
	uint64_t rankAttacks=0;
	uint64_t filesOccupied=~0;
	//uint64_t ranksOccupied=~0;
	for(int i=0;i<8;i++){
		uint64_t f=file(i);
		uint64_t r=rank(i);
		if(!(f&rooks)){
			filesOccupied&=~f;
		}
		/*if(!(r&rooks)){
			ranksOccupied&=~r;
		}*/	
	}
	//print_bitboard(ranksOccupied);
	while(rooks){
		uint64_t rook=firstPiece(rooks);
		rooks = restPieces(rooks);
		uint64_t rookRank=rank(0);
		while(!(rookRank&rook)){
			rookRank<<=1;
		}
		//print_bitboard(rookRank);
		//print_bitboard(occ&rookRank);
		//print_bitboard((occ&rookRank)-(brd<<1));
		rankAttacks|=(((occ&rookRank)-(rook<<1))^occ)&rookRank;
		rankAttacks|=((occ-(rook<<1))^reverse(reverse(occ&rookRank)-(reverse(rook)<<1)))&rookRank;
	}
	uint64_t fileAttacks=((occ-(brd<<1))^reverse(reverse(occ)-(reverse(brd)<<1)))&filesOccupied;
	//brd=restPieces(brd);
	//uint64_t rankAttacks=(((occ&ranksOccupied)-(brd<<1))^reverse(reverse(occ&ranksOccupied)-(reverse(brd)<<1)))&ranksOccupied;
	newSquares|=fileAttacks;
	newSquares|=rankAttacks;
	return newSquares;
}
uint64_t Bitboard::bishopAttacks(uint64_t brd, int blackOrWhite){
	print_bitboard(2);
	print_bitboard(diagonalMask((uint64_t)2,true));
}
uint64_t Bitboard::queenAttacks(uint64_t brd, int blackOrWhite){
	return rookAttacks(brd, blackOrWhite) | bishopAttacks(brd,blackOrWhite);
}
uint64_t Bitboard::firstPiece(uint64_t brd){
	return brd & -brd;
}
uint64_t Bitboard::restPieces(uint64_t brd){
	return brd & (brd - 1);
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
void Bitboard::print_bitboard(uint64_t brd){
	for(int j=7;j>=0;j--){
		for(int i=0;i<8;i++){
			//cout<<i*8+j;
			if(brd>>(i*8+j)&1){
				cout<<"1 ";
			}
			else{
				cout<<". ";
			}
		}
		cout<<endl;
	}
	cout<<endl;
}
void Bitboard::print_binary(uint64_t brd){
	uint64_t mask=(uint64_t)1<<63;
	for(int i=0;i<64;i++){
		if(brd&mask)
			cout<<"1";
		else
			cout<<"0";
		brd<<=1;
	}
	cout<<endl;
}
Bitboard::~Bitboard(){
	free(moveHistory);
	delete [] bitbrds;
}
ostream& operator<<(ostream &strm, Bitboard* board){
	return strm << board->tostring();
}