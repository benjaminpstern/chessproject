
#include "Bitboard.h"
#define num_piece_types 12
#define plyNo moveHistory.size()
const char chars[num_piece_types]={'P','R','N','B','Q','K','p','r','n','b','q','k'};
const int pieceValues[num_piece_types]={1,5,3,3,9,40,-1,-5,-3,-3,-9,-40};
const move_t nullMove=0;
Bitboard::Bitboard(){
	bitbrds=new uint64_t [num_piece_types];
	pieceAttackBrds=new uint64_t [num_piece_types];
	for(int i=0;i<num_piece_types;i++){
		bitbrds[i]=0;
	}
	nodesSearched=0;
	startPosition();
	recalculatePieceAttacks();
}
//1 if white is to move, -1 if black is to move
int Bitboard::toMove(){
	int sideToMove=1;
	if(plyNo%2==1) sideToMove=-1;
	return sideToMove; //1 if white is to move, -1 if black is to move
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
void Bitboard::recalculatePieceAttacks(){
	int sideToMove=toMove();
	int start;
	if(sideToMove==1){
		start=6;
		squaresToBlackKing=0xFFFFFFFFFFFFFFFF;
	}
	else{
		start=0;
		squaresToWhiteKing=0xFFFFFFFFFFFFFFFF;
	}
	for(int i=start;i<start+6;i++){
		pieceAttackBrds[i]=pieceAttacksRecalc(i);
	}
	if(sideToMove==1){
		squaresToWhiteKing = squaresToKingRecalc(0);
	}
	else{
		squaresToBlackKing = squaresToKingRecalc(1);
	}
	start=(start+6)%12;
	for(int i=start;i<start+6;i++){
		pieceAttackBrds[i]=pieceAttacksRecalc(i);
	}
}
uint64_t Bitboard::occupancySet(){
	uint64_t occ=0;
	for(int i=0;i<12;i++){
		occ|=bitbrds[i];
	}
	return occ;
}
//blackOrWhite is 0 if the piece is white or 1 if the piece is blacks
uint64_t Bitboard::ownPieces(int blackOrWhite){
	uint64_t set=0;
	for(int i=myPieces(blackOrWhite);i<myPieces(blackOrWhite)+6;i++){
		//cout<<i<<endl;
		//print_bitboard(bitbrds[i]);
		set|=(bitbrds[i]);
	}
	return set;
}
uint64_t Bitboard::ownPieceAttacks(int blackOrWhite){
	uint64_t set=0;
	for(int i=myPieces(blackOrWhite);i<myPieces(blackOrWhite)+6;i++){
		//cout<<i<<endl;
		//print_bitboard(bitbrds[i]);
		set|=(pieceAttacks(i));
	}
	return set;
}
//blackOrWhite is 0 if the piece is white or 1 if the piece is blacks
uint64_t Bitboard::enemyPieces(int blackOrWhite){
	uint64_t set=0;
	for(int i=otherPieces(blackOrWhite);i<otherPieces(blackOrWhite)+6;i++){
		set|=(bitbrds[i]);
	}
	return set;
}
//attacks of all enemy pieces. blackOrWhite is 0 if the piece is white or 1 if black
uint64_t Bitboard::enemyPieceAttacks(int blackOrWhite){
	uint64_t set=0;
	for(int i=otherPieces(blackOrWhite);i<otherPieces(blackOrWhite)+6;i++){
		set|=(pieceAttacks(i));
	}
	return set;
}
uint64_t Bitboard::pieceAttacks(int pieceIndex){
	return pieceAttackBrds[pieceIndex];
}
//gives all the squares the pieces are attacking on board pieceIndex
uint64_t Bitboard::pieceAttacksRecalc(int pieceIndex){
	switch(pieceIndex){
		case 0:
			return pawnAttacks(bitbrds[pieceIndex],0)&squaresToWhiteKing;
		case 1:
			return rookAttacks(bitbrds[pieceIndex],0)&squaresToWhiteKing;
		case 2:
			return knightAttacks(bitbrds[pieceIndex],0)&squaresToWhiteKing;
		case 3:
			return bishopAttacks(bitbrds[pieceIndex],0)&squaresToWhiteKing;
		case 4:
			return queenAttacks(bitbrds[pieceIndex],0)&squaresToWhiteKing;
		case 5:
			return kingAttacks(bitbrds[pieceIndex],0);
		case 6:
			return pawnAttacks(bitbrds[pieceIndex],1)&squaresToBlackKing;
		case 7:
			return rookAttacks(bitbrds[pieceIndex],1)&squaresToBlackKing;
		case 8:
			return knightAttacks(bitbrds[pieceIndex],1)&squaresToBlackKing;
		case 9:
			return bishopAttacks(bitbrds[pieceIndex],1)&squaresToBlackKing;
		case 10:
			return queenAttacks(bitbrds[pieceIndex],1)&squaresToBlackKing;
		case 11:
			return kingAttacks(bitbrds[pieceIndex],1);
	}
}
//gives all the squares the pieces are attacking on brd, using the type of piece referred to by pieceIndex
uint64_t Bitboard::pieceAttacks(int pieceIndex, uint64_t brd){
	switch(pieceIndex){
		case 0:
			return pawnAttacks(brd,0)&squaresToWhiteKing;
		case 1:
			return rookAttacks(brd,0)&squaresToWhiteKing;
		case 2:
			return knightAttacks(brd,0)&squaresToWhiteKing;
		case 3:
			return bishopAttacks(brd,0)&squaresToWhiteKing;
		case 4:
			return queenAttacks(brd,0)&squaresToWhiteKing;
		case 5:
			return kingAttacks(brd,0);
		case 6:
			return pawnAttacks(brd,1)&squaresToBlackKing;
		case 7:
			return rookAttacks(brd,1)&squaresToBlackKing;
		case 8:
			return knightAttacks(brd,1)&squaresToBlackKing;
		case 9:
			return bishopAttacks(brd,1)&squaresToBlackKing;
		case 10:
			return queenAttacks(brd,1)&squaresToBlackKing;
		case 11:
			return kingAttacks(brd,1);
	}
}
//return true if the move is legal, false otherwise
bool Bitboard::isLegal(move_t m){
	uint64_t newBoard=bitbrds[m.pieceMoved]&((uint64_t)1<<(m.x1*8+m.y1));
	return ((uint64_t)1<<(m.x2*8+m.y2))&pieceAttacks(m.pieceMoved)&~ownPieces(m.pieceMoved/6);
}
bool Bitboard::isCheck(){
	return isInCheck(true)||isInCheck(false);
}
bool Bitboard::isInCheck(int blackOrWhite){
	if(!blackOrWhite)
		return bitbrds[5]&enemyPieceAttacks(0);
	return bitbrds[11]&enemyPieceAttacks(1);
}
bool Bitboard::isCheckmate(){
	std::vector<move_t>* moves=allMoves();
	bool sizeIsZero=moves->size()==0;
	delete moves;
	return sizeIsZero&&isCheck();
}
int Bitboard::checkingPieceIndex(int blackOrWhite){
	uint64_t king=bitbrds[blackOrWhite*6+5];
	int otherSide=(blackOrWhite+1)%2;
	int index=-1;
	for(int i=otherSide*6;i<otherSide*6+5;i++){
		if(pieceAttacks(i)&king){
			if(index!=-1){
				return -1;
			}
			index=i;
		}
	}
	return index;
}
uint64_t Bitboard::squaresToKingRecalc(int blackOrWhite){
	int checkingPiece = checkingPieceIndex(blackOrWhite);
	if(checkingPiece==-1){
		return 0xFFFFFFFFFFFFFFFF;
	}
	uint64_t king = bitbrds[5+6*blackOrWhite];
	uint64_t cPiece = bitbrds[checkingPiece];
	uint64_t firstCheckingPiece;
	uint64_t attacks;
	//cout<<checkingPiece-6*((blackOrWhite+1)%2)<<endl;
	switch(checkingPiece-6*((blackOrWhite+1)%2)){
	case 0:
		return cPiece&pawnAttacks(king,blackOrWhite)&~(king>>1)&~(king<<1)&~(king<<2)&~(king>>2);
	case 1:
		while(cPiece){
			firstCheckingPiece=firstPiece(cPiece);
			attacks=rookAttacks(firstCheckingPiece,(blackOrWhite+1)%2);
			if(attacks&king){
				return (rookAttacks(king,blackOrWhite)&attacks)|firstCheckingPiece;
			}
			cPiece=restPieces(cPiece);
		}
	case 2:
		while(cPiece){
			firstCheckingPiece=firstPiece(cPiece);
			if(knightAttacks(firstCheckingPiece,(blackOrWhite+1)%2)&king){
				return firstCheckingPiece;
			}
			cPiece=restPieces(cPiece);
		}
	case 3:
		while(cPiece){
			firstCheckingPiece=firstPiece(cPiece);
			attacks=bishopAttacks(firstCheckingPiece,(blackOrWhite+1)%2);
			if(attacks&king){
				//print_bitboard(attacks);
				return (bishopAttacks(king,blackOrWhite)&attacks)|firstCheckingPiece;
			}
			cPiece=restPieces(cPiece);
		}
	case 4:
		while(cPiece){
			firstCheckingPiece=firstPiece(cPiece);
			attacks=rookAttacks(firstCheckingPiece,(blackOrWhite+1)%2);
			if(attacks&king){
				return (rookAttacks(king,blackOrWhite)&attacks)|firstCheckingPiece;
			}
			attacks=bishopAttacks(firstCheckingPiece,(blackOrWhite+1)%2);
			if(attacks&king){
				return (bishopAttacks(king,blackOrWhite)&attacks)|firstCheckingPiece;
			}
			cPiece=restPieces(cPiece);
		}
	}
	return 0;
}
uint64_t Bitboard::squaresToKing(int blackOrWhite){
	if(blackOrWhite==0){
		return squaresToWhiteKing;
	}
	return squaresToBlackKing;
}
bool Bitboard::isDraw(){
	if(numPieces(occupancySet())<3){
		return true;
	}
	std::vector<move_t>* movesPtr=allMoves();
	if(movesPtr->size()==0&&!isCheck()){
		delete movesPtr;
		return true;
	}
	delete movesPtr;
	uint64_t originalPieceLocations[12];
	for(int i=0;i<12;i++){
		originalPieceLocations[i]=bitbrds[i];
	}
	uint64_t pieceLocations[12];
	for(int i=0;i<12;i++){
		pieceLocations[i]=bitbrds[i];
	}
	std::vector<move_t>::reverse_iterator rit=moveHistory.rbegin();
	int numMoves = 0;
	int numReps = 1;
	while(rit!=moveHistory.rend()){
		if(numMoves>=50){
			return true;
		}
		move_t m=*rit;
		char pieceMoved = m.getPieceMoved();
		char pieceTaken = m.getPieceTaken();
		if(pieceMoved=='p'||pieceMoved=='P'||pieceTaken!='_'){
			return false;
		}
		int pieceMovedInt=m.pieceMoved;
		pieceLocations[pieceMovedInt]&=~((uint64_t)1<<(m.x2*8+m.y2));//turn off the bit that the piece moved to
		pieceLocations[pieceMovedInt]|=((uint64_t)1<<(m.x1*8+m.y1));//turn on the place that the piece moved from

		bool repetition=true;
		for(int i=0;i<12;i++){
			if(originalPieceLocations[i]!=pieceLocations[i]){
				repetition=false;
			}
		}
		if(repetition){
			numReps++;
			if(numReps>2)
				return true;
		}
		++rit;
		numMoves++;
	}
	return false;
}
/*
 * pointer to vector for all moves of the position
 * the checks and captures will be first and the rest of the moves will come afterward
 */
std::vector<move_t>* Bitboard::allMoves(){
	int sideToMove=toMove();
	std::vector<move_t>* moves = new std::vector<move_t>();
	std::vector<move_t> captures;
	std::vector<move_t> nonCaptures;
	int i=0;
	if(sideToMove<0) i=6;
	int max=i+6;
	for(;i<max;i++){
		std::vector<move_t>* pieceMovesPtr=allMoves(i);
		std::vector<move_t> pieceMoves=*pieceMovesPtr;
		int j;
		for(j=0;j<pieceMoves.size()&&pieceMoves[j];j++){
			moves->push_back(pieceMoves[j]);
		}
		for(j++;j<pieceMoves.size()&&pieceMoves[j].getPieceTaken()!='_';j++){
			//cout<<j<<endl;
			captures.push_back(pieceMoves[j]);
		}
		for(;j<pieceMoves.size();j++){
			nonCaptures.push_back(pieceMoves[j]);
		}
		delete pieceMovesPtr;
	}
	for(int j=0;j<captures.size();j++){
		moves->push_back(captures[j]);
	}
	for(int j=0;j<nonCaptures.size();j++){
		moves->push_back(nonCaptures[j]);
	}
	return moves;
}
//returns the moves of a piece, with the checks first, then a nullMove, then captures, then rest of the moves
std::vector<move_t>* Bitboard::allMoves(int pieceIndex){
	uint64_t first=firstPiece(bitbrds[pieceIndex]);
	uint64_t rest=restPieces(bitbrds[pieceIndex]);
	uint64_t own = ownPieces(pieceIndex/6);
	uint64_t enemy = enemyPieces(pieceIndex/6);
	int notBlackOrWhite=((pieceIndex/6)+1)%2;
	uint64_t enemyKing=bitbrds[notBlackOrWhite*6+5];
	uint64_t checkSquares;
	switch(pieceIndex%6){
	case 0:
		checkSquares=pawnAttacks(enemyKing,0xFFFFFFFFFFFFFFFF,notBlackOrWhite);
		break;
	case 1:
		checkSquares=rookAttacks(enemyKing,notBlackOrWhite);
		break;
	case 2:
		checkSquares=knightAttacks(enemyKing,notBlackOrWhite);
		break;
	case 3:
		checkSquares=bishopAttacks(enemyKing,notBlackOrWhite);
		break;
	case 4:
		checkSquares=queenAttacks(enemyKing,notBlackOrWhite);
		break;
	case 5:
		checkSquares=0;
	}
	std::vector<move_t>* moves=new std::vector<move_t>();
	std::vector<move_t> captures;
	std::vector<move_t> nonCaptures;
	while(first){
		uint64_t pieceMoves=pieceAttacks(pieceIndex,first)&~own;
		uint64_t checks=pieceMoves&checkSquares;
		pieceMoves &= ~checks;
		uint64_t pieceCaptures = enemy&pieceMoves;
		pieceMoves &= ~enemy;
		while(checks){
			uint64_t firstMove=firstPiece(checks);
			int x=xValue(firstMove);
			int y=yValue(firstMove);
			moves->push_back(move_t(xValue(first),yValue(first),x,y,chars[pieceIndex],piece(x,y)));
			checks=restPieces(checks);
		}
		while(pieceCaptures){
			uint64_t firstMove=firstPiece(pieceCaptures);
			int x=xValue(firstMove);
			int y=yValue(firstMove);
			captures.push_back(move_t(xValue(first),yValue(first),x,y,chars[pieceIndex],piece(x,y)));
			pieceCaptures=restPieces(pieceCaptures);
		}
		while(pieceMoves){
			uint64_t firstMove=firstPiece(pieceMoves);
			int x=xValue(firstMove);
			int y=yValue(firstMove);
			nonCaptures.push_back(move_t(xValue(first),yValue(first),x,y,chars[pieceIndex],'_'));
			pieceMoves=restPieces(pieceMoves);
		}
		first=firstPiece(rest);
		rest=restPieces(rest);
	}
	moves->push_back(nullMove);
	for(int i=0;i<captures.size();i++){
		moves->push_back(captures[i]);
	}
	for(int i=0;i<nonCaptures.size();i++){
		moves->push_back(nonCaptures[i]);
	}
	return moves;
}
//make the move on the board, return true if successful
bool Bitboard::move(move_t m){
	bitbrds[m.pieceMoved]&=~((uint64_t)1<<(m.x1*8+m.y1));//turn off the bit that the piece moved from
	bitbrds[m.pieceMoved]|=((uint64_t)1<<(m.x2*8+m.y2));//turn on the place that the piece moved to
	if(m.pieceTaken!=12){
		bitbrds[m.pieceTaken]&=~(uint64_t)0^((uint64_t)1<<(m.x2*8+m.y2));//turn off the place where the piece got taken
	}
	if(m.pieceMoved==0&&m.y2==7){
		bitbrds[0]&=~((uint64_t)1<<(m.x2*8+m.y2));
		bitbrds[4]|=((uint64_t)1<<(m.x2*8+m.y2));
	}
	if(m.pieceMoved==6&&m.y2==0){
		bitbrds[6]&=~((uint64_t)1<<(m.x2*8+m.y2));
		bitbrds[10]|=((uint64_t)1<<(m.x2*8+m.y2));
	}
	moveHistory.push_back(m);
	recalculatePieceAttacks();
	return true;
}
//take back the previous move, return true if successful
bool Bitboard::takeBack(){
	move_t m=moveHistory.back();
	moveHistory.pop_back();
	bitbrds[m.pieceMoved] |= ((uint64_t)1<<(m.x1*8+m.y1));
	bitbrds[m.pieceMoved] &= ~((uint64_t)1<<(m.x2*8+m.y2));
	if(m.pieceTaken!=12){
		bitbrds[m.pieceTaken]|=(uint64_t)0^((uint64_t)1<<(m.x2*8+m.y2));//turn on the place where the piece got taken
	}

	//cout<<plyNo<<endl;
	recalculatePieceAttacks();
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
//gets the value of the piece on that square
int Bitboard::pieceValue(int x,int y){
	for(int i=0;i<num_piece_types;i++){
		if((bitbrds[i]>>(x*CHAR_BIT+y))&1){
			pieceValues[i];
		}
	}
	return 0;
}
//return a zero-terminated list of all moves that have been played in the game.
std::vector<move_t> Bitboard::getMoveHistory(){
	return moveHistory;
}
uint64_t Bitboard::knightAttacks(uint64_t brd,int blackOrWhite){
	uint64_t newSquares=0;
	int sideToMove=toMove();
	if((sideToMove==1&&blackOrWhite==0)||(sideToMove==-1&&blackOrWhite==1)){
		uint64_t filePinned=filePins(blackOrWhite)&brd;
		brd ^= filePinned;
		uint64_t diagPinned=diagPins(blackOrWhite)&brd;
		brd ^= diagPinned;
	}
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
	return pawnAttacks(brd,occupancySet(),blackOrWhite);
}
uint64_t Bitboard::pawnAttacks(uint64_t brd, uint64_t occ, int blackOrWhite){
	uint64_t pushSquares=0;//the squares where the pawn would be pushing
	uint64_t takeSquares=0;//the squares where the pawn would be taking
	int forward=1;
	if(blackOrWhite>0){
		forward=-1;
	}
	uint64_t filePinned=0;
	uint64_t diagPinned=0;
	int sideToMove=toMove();
	if((sideToMove==1&&blackOrWhite==0)||(sideToMove==-1&&blackOrWhite==1)){
		filePinned=filePins(blackOrWhite)&brd;
		brd ^= filePinned;
		diagPinned=diagPins(blackOrWhite)&brd;
		brd ^= diagPinned;
	}
	uint64_t mask=rank(3);
	if(forward==-1)
		mask<<=1;
	if(forward==1){
		pushSquares|=brd<<1;
		takeSquares|=brd<<(1+CHAR_BIT);
		takeSquares|=brd>>(CHAR_BIT-1);
		uint64_t doublePush=(brd<<2)&mask;
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
	pushSquares &=(~occ);
	takeSquares &= occ;
	uint64_t newSquares=pushSquares|takeSquares;
	uint64_t king = bitbrds[blackOrWhite*6+5];
	while(diagPinned){
		uint64_t first=firstPiece(diagPinned);
		newSquares|=(pawnAttacksBypassPins(first,blackOrWhite)&diagPinMask(first,king));
		diagPinned=restPieces(diagPinned);
	}
	while(filePinned){
		uint64_t first=firstPiece(filePinned);
		newSquares|=(pawnAttacksBypassPins(first,blackOrWhite)&filePinMask(first,king));
		filePinned=restPieces(filePinned);
	}
	return newSquares;
}
uint64_t Bitboard::pawnAttacksBypassPins(uint64_t brd, int blackOrWhite){
	uint64_t pushSquares=0;//the squares where the pawn would be pushing
	uint64_t takeSquares=0;//the squares where the pawn would be taking
	int forward=1;
	if(blackOrWhite>0){
		forward=-1;
	}
	uint64_t occ=occupancySet();
	uint64_t mask=rank(3);
	if(forward==-1)
		mask<<=1;
	if(forward==1){
		pushSquares|=brd<<1;
		takeSquares|=brd<<(1+CHAR_BIT);
		takeSquares|=brd>>(CHAR_BIT-1);
		uint64_t doublePush=(brd<<2)&mask;
		pushSquares|=(doublePush&(~occ&~(occ<<1)));
		//print_bitboard((~occ&~(occ<<1)));
	}
	else{
		pushSquares|=brd>>1;
		takeSquares|=brd>>(1+CHAR_BIT);
		takeSquares|=brd<<(CHAR_BIT-1);
		uint64_t doublePush=(brd>>2)&mask;
		uint64_t occ=occ;
		pushSquares|=(doublePush&(~occ&~(occ>>1)));
	}
	pushSquares &=(~occ);
	takeSquares &= occ;
	uint64_t newSquares=pushSquares|takeSquares;
	//print_bitboard(brd);
	return newSquares;
}
uint64_t Bitboard::kingAttacks(uint64_t brd, int blackOrWhite){
	uint64_t newSquares=0;
	uint64_t taboo=0;
	uint64_t occ=occupancySet();
	newSquares|=(brd<<1)&notFirstRank;
	newSquares|=(brd>>1)&notLastRank;
	newSquares|=(brd<<7)&notLastRank;
	newSquares|=(brd>>7)&notFirstRank;
	newSquares|=(brd<<8);
	newSquares|=(brd>>8);
	newSquares|=(brd<<9)&notFirstRank;
	newSquares|=(brd>>9)&notLastRank;
	int others=otherPieces(blackOrWhite);
	for(int i=others;i<others+5;i++){//to exclude the enemy king
		if(i==others||i==others+1||i==others+3||i==others+4){
			if(i==others)
				taboo|=pawnAttacks(bitbrds[i],0xFFFFFFFFFFFFFFFF,(blackOrWhite+1)%2);
			else if(i==others+1)
				taboo|=rookAttacks(bitbrds[i],occ&~brd,(blackOrWhite+1)%2);
			else if(i == others+3)
				taboo|=bishopAttacks(bitbrds[i],occ&~brd,(blackOrWhite+1)%2);
			else if(i==others+4)
				taboo|=queenAttacks(bitbrds[i],occ&~brd,(blackOrWhite+1)%2);
		}
		else{
			taboo|=pieceAttacks(i);
		}
	}
	uint64_t otherKing=bitbrds[((blackOrWhite+1)%2)*6+5];
	uint64_t otherKingMoves=0;
	otherKingMoves|=(otherKing<<1)&notFirstRank;
	otherKingMoves|=(otherKing>>1)&notLastRank;
	otherKingMoves|=(otherKing<<7)&notLastRank;
	otherKingMoves|=(otherKing>>7)&notFirstRank;
	otherKingMoves|=(otherKing<<8);
	otherKingMoves|=(otherKing>>8);
	otherKingMoves|=(otherKing<<9)&notFirstRank;
	otherKingMoves|=(otherKing>>9)&notLastRank;
	taboo|=otherKingMoves;
	uint64_t mask=((uint64_t)1<<(2*(CHAR_BIT)))|((uint64_t)1<<(6*(CHAR_BIT)));
	mask|=mask<<7;
	uint64_t betweenSquares=0x0081810081810000;
	betweenSquares&=(~occ);
	betweenSquares&=(betweenSquares>>8)|file(6)|file(3);
	betweenSquares&=(betweenSquares<<8)|file(2)|file(5);
	newSquares|=(brd>>(2*CHAR_BIT))&mask&betweenSquares;
	newSquares|=(brd<<(2*CHAR_BIT))&mask&betweenSquares;
	newSquares&=~taboo;
	return newSquares;
}
uint64_t Bitboard::rookAttacks(uint64_t brd, int blackOrWhite){
	return rookAttacks(brd,occupancySet(),blackOrWhite);
}
uint64_t Bitboard::rookAttacks(uint64_t brd, uint64_t occ, int blackOrWhite){
	uint64_t newSquares=0;
	int sideToMove=toMove();
	uint64_t filePinned=0;
	uint64_t diagPinned=0;
	if((sideToMove==1&&blackOrWhite==0)||(sideToMove==-1&&blackOrWhite==1)){
		filePinned=filePins(blackOrWhite)&brd;
		brd ^= filePinned;
		diagPinned=diagPins(blackOrWhite)&brd;
		brd ^= diagPinned;
	}
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
	while(rooks){
		uint64_t rook=firstPiece(rooks);
		rooks = restPieces(rooks);
		uint64_t rookRank=rank(0);
		while(!(rookRank&rook)){
			rookRank<<=1;
		}
		rankAttacks|=(((occ&rookRank)-(rook<<1))^occ)&rookRank;
		rankAttacks|=((occ-(rook<<1))^reverse(reverse(occ&rookRank)-(reverse(rook)<<1)))&rookRank;
	}
	uint64_t fileAttacks=((occ-(brd<<1))^reverse(reverse(occ)-(reverse(brd)<<1)))&filesOccupied;
	newSquares|=fileAttacks;
	newSquares|=rankAttacks;
	uint64_t king=bitbrds[6*blackOrWhite+5];
	while(filePinned){
		uint64_t first = firstPiece(filePinned);
		newSquares|=(rookAttacksBypassPins(first,occ,blackOrWhite)&filePinMask(first,king));
		filePinned=restPieces(filePinned);
	}
	return newSquares;
}
uint64_t Bitboard::rookAttacksBypassPins(uint64_t brd, uint64_t occ, int blackOrWhite){
	uint64_t newSquares=0;
	int sideToMove=toMove();
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
	return bishopAttacks(brd, occupancySet(),blackOrWhite);
}
/*
 *
 */
uint64_t Bitboard::bishopAttacks(uint64_t brd, uint64_t occ, int blackOrWhite){
	uint64_t newSquares = 0;
	uint64_t forward;
	uint64_t rev;
	uint64_t attacks;
	uint64_t filePinned=0;
	uint64_t diagPinned=0;
	int sideToMove=toMove();
	if((sideToMove==1&&blackOrWhite==0)||(sideToMove==-1&&blackOrWhite==1)){
		filePinned=filePins(blackOrWhite)&brd;
		brd ^= filePinned;
		diagPinned=diagPins(blackOrWhite)&brd;
	}
	uint64_t lineMask1;
	uint64_t lineMask2;
	uint64_t bishop;
	uint64_t bishops=brd;
	while(bishops){
		//print_bitboard(bishops);
		bishop = firstPiece(bishops);
		bishops = restPieces(bishops);
		lineMask1 = diagonalMask(bishop,true);
		lineMask2 = diagonalMask(bishop,false);
		forward = occ & lineMask1;
		rev = byteSwap(forward);
		forward -= bishop;
		rev -= byteSwap(bishop);
		forward ^= byteSwap(rev);
		newSquares |= (forward & lineMask1);
		forward = occ & lineMask2;
		rev = byteSwap(forward);
		forward -= bishop;
		rev -= byteSwap(bishop);
		forward ^= byteSwap(rev);
		attacks = (forward & lineMask2);
		if(bishop&diagPinned){
			attacks&=diagPinMask(bishop,bitbrds[6*blackOrWhite+5]);
		}
		newSquares|=attacks;
	}
	//cout<<1<<endl;
	return newSquares;
	
}
uint64_t Bitboard::queenAttacks(uint64_t brd, uint64_t occ, int blackOrWhite){
	uint64_t newSquares=rookAttacks(brd, occ, blackOrWhite) | bishopAttacks(brd, occ, blackOrWhite);
	return newSquares;
}
uint64_t Bitboard::queenAttacks(uint64_t brd,int blackOrWhite){
	return queenAttacks(brd,occupancySet(),blackOrWhite);
}
uint64_t Bitboard::filePins(int blackOrWhite){
	uint64_t king=bitbrds[blackOrWhite*6+5];
	int notBlackOrWhite=(blackOrWhite+1)%2;
	uint64_t occ=occupancySet();
	uint64_t pieces=ownPieces(blackOrWhite)&(~king);
	uint64_t pinners = bitbrds[notBlackOrWhite*6+1]|bitbrds[notBlackOrWhite*6+4];
	uint64_t first;
	uint64_t pinned=0;
	uint64_t checkers=pinners;
	while(checkers){
		first=firstPiece(checkers);
		if((rookAttacks(first,occ,notBlackOrWhite)&king)){
			pinners^=first;
		}
		checkers=restPieces(checkers);
	}
	while(pieces){
		first=firstPiece(pieces);
		pieces=restPieces(pieces);
		if(rookAttacks(pinners,occ&(~first),notBlackOrWhite)&king){
			pinned |= first;
		}
	}
	return pinned;
}
uint64_t Bitboard::diagPins(int blackOrWhite){
	uint64_t king=bitbrds[blackOrWhite*6+5];
	int notBlackOrWhite=(blackOrWhite+1)%2;
	uint64_t occ=occupancySet();
	uint64_t pieces=ownPieces(blackOrWhite)&(~king);
	uint64_t pinners = bitbrds[notBlackOrWhite*6+3]|bitbrds[notBlackOrWhite*6+4];
	uint64_t first;
	uint64_t pinned=0;
	uint64_t checkers=pinners;
	while(checkers){
		first=firstPiece(checkers);
		if((bishopAttacks(first,occ,notBlackOrWhite)&king)){
			pinners^=first;
		}
		checkers=restPieces(checkers);
	}
	while(pieces){
		first=firstPiece(pieces);
		pieces=restPieces(pieces);
		if(bishopAttacks(pinners,occ&(~first),notBlackOrWhite)&king){
			pinned |= first;
		}
	}
	return pinned;
}
uint64_t Bitboard::firstPiece(uint64_t brd){
	return brd & -brd;
}
uint64_t Bitboard::restPieces(uint64_t brd){
	return brd & (brd - 1);
}
//the amount of stuff that's hanging.
//If there's a lot of stuff hanging the program should go deeper to look for the right moves
int Bitboard::hangingPieces(){
	int sideToMove=toMove();
	int blackOrWhite=0;
	int enemyColor=1;
	uint64_t defense=ownPieceAttacks(blackOrWhite);
	if(sideToMove<0){
		blackOrWhite=1;
		enemyColor=0;
	} 
	int hangingValues=0;
	for(int i=blackOrWhite*6;i<blackOrWhite*6+6;i++){
		for(int j=enemyColor*6;j<enemyColor*6+6;j++){
			uint64_t hanging = bitbrds[i]&pieceAttacks(j);
			while(hanging){
				uint64_t first=firstPiece(hanging);
				hangingValues+=pieceValues[i];
				if(hanging&defense){
					hangingValues+=pieceValues[j];
				}
				hanging=restPieces(hanging);
			}
		}
		//cout<<hanging<<endl;
		return hangingValues;
	}
}
//should get an array with the n best moves.
std::vector<move_t>* Bitboard::nBestMoves(int n){
	nodesSearched=0;
	int sideToMove=toMove();
	std::vector<move_t>* bestMovesPtr=new std::vector<move_t>();
	std::vector<move_t> bestMoves = *bestMovesPtr;
	std::vector<move_t>* movesPtr = allMoves();
	std::vector<move_t> moves = *movesPtr;
	for(int i=0;i<n;i++){
		bestMoves.push_back(nullMove);
		bestMoves[i].changeEvaluation(40*sideToMove*-1);
		//cout<<bestMoves[i].getEvaluation()*sideToMove<<endl;
	}
	for(int i=0;moves[i]!=nullMove;i++){
		//cout<<moves[i].tostring();
		move(moves[i]);
		nodesSearched++;
		//cout<<this;
		move_t m=moves[i];
		moves[i].changeEvaluation(alphaBeta(-100*sideToMove,100*sideToMove,0));
		takeBack();
		//cout<<this;
	}
	for(int i=0;i<moves.size();i++){
		//cout<<moves[i].getEvaluation()*sideToMove<<" "<<bestMoves[n-1].getEvaluation()*sideToMove<<endl;
		if (moves[i].getEvaluation()*sideToMove > (*bestMovesPtr)[n-1].getEvaluation()*sideToMove){
			int j;
			for(j=n-1;j>=0&&moves[i].getEvaluation()*sideToMove > (*bestMovesPtr)[j].getEvaluation()*sideToMove;j--){
				if(j!=0){
					(*bestMovesPtr)[j]=(*bestMovesPtr)[j-1];
				}
				else{
					(*bestMovesPtr)[j]=moves[i];
				}
				//cout<<j<<endl;
			}
		}
	}
	delete movesPtr;
	/*cout<<"searched "<<nodesSearched<<" nodes"<<endl;
	cout<<"evaluation is now "<<(*bestMovesPtr)[0].getEvaluation()<<endl;
	for(int i=0;i<bestMovesPtr->size();i++){
		cout<<(*bestMovesPtr)[i].tostring();
	}*/
	return bestMovesPtr;
}
//evaluates the position nonrecursively
double Bitboard::evaluate(){
	double evaluation=0;
	for(int i=0;i<12;i++){
		evaluation+=numPieces(bitbrds[i])*pieceValues[i];
		if(i==0||i==6){
			evaluation+=.01*numPieces((pieceAttacks(i)|bitbrds[i])&centralSquares);
		}
		else{
			evaluation+=.01*numPieces(pieceAttacks(i))*10/pieceValues[i];
		}
	}
	//cout<<this<<evaluation<<endl;
	return evaluation;
}
//evaluates the position up to a minimum depth of depth

double Bitboard::evaluate(int depth){
	if(bitbrds[5]==0) return -40;
	if(bitbrds[11]==0) return 40;
	double eval=evaluate();
	int hanging=hangingPieces();
	if(depth<1){
		return eval;
	}
	int sideToMove=toMove();
	double bestEvaluation=40*sideToMove*-1;
	std::vector<move_t>* movesPtr = allMoves();
	std::vector<move_t> moves = *movesPtr;
	int i=0;
	double bestCaptureEval=40*sideToMove*-1;
	for(i=0;moves[i].isCapture();i++){
		move(moves[i]);
		nodesSearched++;
		if(nodesSearched%10000==0){
			cout<<"searching node "<<nodesSearched<<endl;
			cout<<"depth is "<<depth<<endl;
			cout<<this;
		}
		double newEval=evaluate(depth);
		if(newEval*sideToMove>bestCaptureEval*sideToMove){
			bestCaptureEval=newEval;
		}
		takeBack();
	}
	if(bestCaptureEval*sideToMove>eval*sideToMove+1){
		delete movesPtr;
		return bestCaptureEval;
	}
	for(;moves[i];i++){
		move(moves[i]);
		moves[i].changeEvaluation(evaluate(depth-1));
		nodesSearched++;
		if(nodesSearched%10000==0){
			cout<<"searching node "<<nodesSearched<<endl;
		}
		takeBack();
	}
	for(i=0;moves[i];i++){
		if (moves[i].getEvaluation()*sideToMove > bestEvaluation*sideToMove){
			bestEvaluation = moves[i].getEvaluation();
		}
	}
	delete movesPtr;
	return bestEvaluation;
}
/*
int Quiesce( int alpha, int beta ) {
    int stand_pat = Evaluate();
    if( stand_pat >= beta )
        return beta;
    if( alpha < stand_pat )
        alpha = stand_pat;
 
    until( every_capture_has_been_examined )  {
        MakeCapture();
        score = -Quiesce( -beta, -alpha );
        TakeBackMove();
 
        if( score >= beta )
            return beta;
        if( score > alpha )
           alpha = score;
    }
    return alpha;
}
*/
double Bitboard::quiesce(double alpha, double beta){
	int sideToMove=toMove();
	double standPat=evaluate();
	double score;
	if(standPat*sideToMove>=beta*sideToMove)
		cout<<"rejected"<<endl;
		cout<<this;
		return beta;
	if(alpha*sideToMove < standPat*sideToMove)
		alpha=standPat;
	std::vector<move_t>* movesPtr=allMoves();
	std::vector<move_t> moves=*movesPtr;
	for(int i=0;i<moves.size();i++){
		nodesSearched++;
		move(moves[i]);
		if(isCheck()||moves[i].getPieceTaken()!='_'){
			score = -quiesce( -beta, -alpha );
		}
		else{
			score = standPat;
		}
		takeBack();
		if( score*sideToMove>= beta*sideToMove ){
			delete movesPtr;
			return beta;
		}
		if( score*sideToMove > alpha*sideToMove )
			alpha = score;

	}
	delete movesPtr;
	return alpha;
}
/*int alphaBeta( int alpha, int beta, int depthleft ) {
   if( depthleft == 0 ) return quiesce( alpha, beta );
   for ( all moves)  {
      score = -alphaBeta( -beta, -alpha, depthleft - 1 );
      if( score >= beta )
         return beta;   //  fail hard beta-cutoff
      if( score > alpha )
         alpha = score; // alpha acts like max in MiniMax
   }
   return alpha;
}*/
double Bitboard::alphaBeta(double alpha, double beta, int depth){
	int sideToMove=toMove();
	if(depth == 0) return quiesce( alpha, beta );
	double score;
	std::vector<move_t>* movesPtr=allMoves();
	std::vector<move_t> moves=*movesPtr;
	for(int i=0;i<moves.size();i++){
		nodesSearched++;
		score=-alphaBeta( -beta, -alpha, depth-1);
		if(score*sideToMove >= beta*sideToMove){
			delete movesPtr;
			return beta;
		}
		if( score*sideToMove > alpha*sideToMove )
			alpha = score;
	}
	delete movesPtr;
	return alpha;
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
	delete [] bitbrds;
	delete [] pieceAttackBrds;
}
ostream& operator<<(ostream &strm, Bitboard* board){
	return strm << board->tostring();
}
