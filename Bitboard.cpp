
#include "Bitboard.h"
#define num_piece_types 12
#define plyNo moveHistory.size()
#define TIMEUP 100000
const char chars[num_piece_types]={'P','R','N','B','Q','K','p','r','n','b','q','k'};
const int pieceValues[num_piece_types]={1,5,3,3,9,40,-1,-5,-3,-3,-9,-40};
move_t nullMove=0;
Bitboard::Bitboard(){
	bitbrds=new uint64_t [num_piece_types];
	whiteKCastle = -1;
	whiteQCastle = -1;
	blackKCastle = -1;
	blackQCastle = -1;
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
	move_t lastMove;
	if(moveHistory.size() == 0){
		lastMove = nullMove;
	}
	else{
		lastMove = moveHistory.back();
	}
	if(sideToMove==1){
		start=6;
		squaresToBlackKing=0xFFFFFFFFFFFFFFFF;
	}
	else{
		start=0;
		squaresToWhiteKing=0xFFFFFFFFFFFFFFFF;
	}
	for(int i=start;i<start+6;i++){
		if(bitbrds[i]!=0){
			pieceAttackBrds[i]=pieceAttacksRecalc(i);
		}
	}
	if(sideToMove==1){
		squaresToWhiteKing = squaresToKingRecalc(0);
	}
	else{
		squaresToBlackKing = squaresToKingRecalc(1);
	}
	start=(start+6)%12;
	for(int i=start;i<start+6;i++){
		if(bitbrds[i]!=0){
			pieceAttackBrds[i]=pieceAttacksRecalc(i);
		}
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
		//print_bitboard(bitbrds[i]);
		set|=(bitbrds[i]);
	}
	return set;
}
uint64_t Bitboard::ownPieceAttacks(int blackOrWhite){
	uint64_t set=0;
	for(int i=myPieces(blackOrWhite);i<myPieces(blackOrWhite)+6;i++){
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
	if(m == nullMove) {
		return false;
	}
	//uint64_t newBoard=bitbrds[m.pieceMoved]&((uint64_t)1<<(m.x1*8+m.y1));
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
	clock_t ticks = clock();
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
		for(j=0;j<pieceMoves.size()&&pieceMoves[j].getPieceTaken()!='_';j++){
			captures.push_back(pieceMoves[j]);
		}
		std::sort(pieceMovesPtr->begin(),pieceMovesPtr->begin()+j,nullMove.captureComp);
		for(;j<pieceMoves.size();j++){
			nonCaptures.push_back(pieceMoves[j]);
		}
		std::sort(pieceMovesPtr->begin(),pieceMovesPtr->begin()+j,nullMove.centralComp);
		delete pieceMovesPtr;
	}
	for(int j=0;j<captures.size();j++){
		moves->push_back(captures[j]);
	}
	for(int j=0;j<nonCaptures.size();j++){
		moves->push_back(nonCaptures[j]);
	}
	//cout<<"AllMoves took "<<((double)(clock()-ticks)/CLOCKS_PER_SEC)<<" seconds"<<endl;
	return moves;
}
std::vector<move_t>* Bitboard::allCaptures(){
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
	return moves;
}
//returns the moves of a piece, with the checks first, then a nullMove, then captures, then rest of the moves
std::vector<move_t>* Bitboard::allMoves(int pieceIndex){
	uint64_t first=firstPiece(bitbrds[pieceIndex]);
	uint64_t rest=restPieces(bitbrds[pieceIndex]);
	uint64_t own = ownPieces(pieceIndex/6);
	uint64_t enemy = enemyPieces(pieceIndex/6);
	int notBlackOrWhite=((pieceIndex/6)+1)%2;
	std::vector<move_t>* moves=new std::vector<move_t>();
	std::vector<move_t> captures;
	std::vector<move_t> nonCaptures;
	while(first){
		uint64_t pieceMoves=pieceAttacks(pieceIndex,first)&~own;
		uint64_t pieceCaptures = enemy&pieceMoves;
		pieceMoves &= ~enemy;
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
	//cout<<m.tostring()<<endl;
	if(m == nullMove){
		moveHistory.push_back(m);
		return true;
	}
	bitbrds[m.pieceMoved]&=~((uint64_t)1<<(m.x1*8+m.y1));//turn off the bit that the piece moved from
	bitbrds[m.pieceMoved]|=((uint64_t)1<<(m.x2*8+m.y2));//turn on the place that the piece moved to
	if(m.pieceTaken!=12){
		bitbrds[m.pieceTaken]&=~(uint64_t)0^((uint64_t)1<<(m.x2*8+m.y2));//turn off the place where the piece got taken
	}
	if(m.pieceMoved == 5 && m.x1 == 4 && m.x2 == 2){
		bitbrds[1] |= ((uint64_t)1 << (3 * 8 + 0));
		bitbrds[1] &= ~((uint64_t)1 << (0 * 8 + 0));
	}
	if(m.pieceMoved == 5 && m.x1 == 4 && m.x2 == 6){
		bitbrds[1] |= ((uint64_t)1 << (5 * 8 + 0));
		bitbrds[1] &= ~((uint64_t)1 << (7 * 8 + 0));
	}
	if(m.pieceMoved == 11 && m.x1 == 4 && m.x2 == 2){
		bitbrds[7] |= ((uint64_t)1 << (3 * 8 + 7));
		bitbrds[7] &= ~((uint64_t)1 << (0 * 8 + 7));
	}
	if(m.pieceMoved == 11 && m.x1 == 4 && m.x2 == 6){
		bitbrds[7] |= ((uint64_t)1 << (5 * 8 + 7));
		bitbrds[7] &= ~((uint64_t)1 << (7 * 8 + 7));
	}
	if(m.pieceMoved == 0 && m.y2 == 7){
		bitbrds[0]&=~((uint64_t)1 << (m.x2 * 8 + m.y2));
		bitbrds[4]|=((uint64_t)1 << (m.x2 * 8 + m.y2));
	}
	if(m.pieceMoved==6&&m.y2==0){
		bitbrds[6]&=~((uint64_t)1<<(m.x2*8+m.y2));
		bitbrds[10]|=((uint64_t)1<<(m.x2*8+m.y2));
	}
	moveHistory.push_back(m);
	if(m.pieceMoved == 1 && m.x1 == 0 && m.y1 == 0 && whiteQCastle == -1){
		whiteQCastle = moveHistory.size();
	}
	if(m.pieceMoved == 1 && m.x1 == 7 && m.y1 == 0 && whiteKCastle == -1){
		whiteKCastle = moveHistory.size();
	}
	if(m.pieceMoved == 7 && m.x1 == 0 && m.y1 == 7 && blackQCastle == -1){
		blackQCastle = moveHistory.size();
	}
	if(m.pieceMoved == 7 && m.x1 == 7 && m.y1 == 7 && blackKCastle == -1){
		blackKCastle = moveHistory.size();
	}
	if(m.pieceMoved == 5){
		if(whiteKCastle == -1){
			whiteKCastle = moveHistory.size();
		}
		if(whiteQCastle == -1){
			whiteQCastle = moveHistory.size();
		}
	}
	if(m.pieceMoved == 11){
		if(blackKCastle == -1){
			blackKCastle = moveHistory.size();
		}
		if(blackQCastle == -1){
			blackQCastle = moveHistory.size();
		}
	}
	recalculatePieceAttacks();
	//cout<<this<<endl;
	return true;
}
//take back the previous move, return true if successful
bool Bitboard::takeBack(){
	move_t m=moveHistory.back();
	moveHistory.pop_back();
	if(m == nullMove){
		return true;
	}
	if(whiteKCastle > (int)moveHistory.size()){
		whiteKCastle = -1;
	}
	if(whiteQCastle > (int)moveHistory.size()){
		whiteQCastle = -1;
	}
	if(blackKCastle > (int)moveHistory.size()){
		blackKCastle = -1;
	}
	if(blackQCastle > (int)moveHistory.size()){
		blackQCastle = -1;
	}
	bitbrds[m.pieceMoved] |= ((uint64_t)1<<(m.x1*8+m.y1));
	bitbrds[m.pieceMoved] &= ~((uint64_t)1<<(m.x2*8+m.y2));
	if(m.pieceMoved == 0 && m.y2 == 7){
		bitbrds[4] &= ~((uint64_t)1<<(m.x2*8+m.y2));
	}
	if(m.pieceMoved == 6 && m.y2 == 0){
		bitbrds[10] &= ~((uint64_t)1<<(m.x2*8+m.y2));
	}
	if(m.pieceTaken!=12){
		bitbrds[m.pieceTaken]|=(uint64_t)0^((uint64_t)1<<(m.x2*8+m.y2));//turn on the place where the piece got taken
	}
	if(m.pieceMoved == 5 && m.x1 == 4 && m.x2 == 6){
		bitbrds[1] &= ~((uint64_t)1 << (5 * 8 + 0));
		bitbrds[1] |= ((uint64_t)1 << (7 * 8 + 0));
	}
	if(m.pieceMoved == 5 && m.x1 == 4 && m.x2 == 2){
		bitbrds[1] &= ~((uint64_t)1 << (3 * 8 + 0));
		bitbrds[1] |= ((uint64_t)1 << (0 * 8 + 0));
	}
	if(m.pieceMoved == 11 && m.x1 == 4 && m.x2 == 6){
		bitbrds[7] &= ~((uint64_t)1 << (5 * 8 + 7));
		bitbrds[7] |= ((uint64_t)1 << (7 * 8 + 7));
	}
	if(m.pieceMoved == 11 && m.x1 == 4 && m.x2 == 2){
		bitbrds[7] &= ~((uint64_t)1 << (3 * 8 + 7));
		bitbrds[7] |= ((uint64_t)1 << (0 * 8 + 7));
	}

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
	newSquares|=(brd>>7)&notFirstRank;
	newSquares|=(brd<<7)&notLastRank;
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
	uint64_t betweenSquares=0x0081510081510000;
	betweenSquares&=(~occ);
	betweenSquares&=(betweenSquares>>8)|file(6)|file(3);
	betweenSquares&=(betweenSquares<<8)|file(2)|file(5);
	if(((blackOrWhite == 0 && whiteQCastle == -1) || (blackOrWhite == 1 && blackQCastle == -1)) && !(occ & brd<<(3*CHAR_BIT))){
		newSquares|=(brd>>(2*CHAR_BIT))&mask&betweenSquares;
	}
	if((blackOrWhite == 0 && whiteKCastle == -1)||(blackOrWhite == 1 && blackKCastle == -1) ){
		newSquares|=(brd<<(2*CHAR_BIT))&mask&betweenSquares;
	}
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
	rooks = brd;
	uint64_t fileAttacks = 0;
	while(rooks){
		uint64_t rookFile = file(0);
		uint64_t rook = firstPiece(rooks);
		rooks = restPieces(rooks);
		while(!(rookFile&rook)){
			rookFile<<=8;
		}
		fileAttacks |= ((occ-(rook<<1))^reverse(reverse(occ)-(reverse(rook)<<1)))&rookFile;
	}
	/*if(occ != occupancySet()){
		cout<<"Occ"<<endl;
		print_bitboard(occ);
		cout<<"file attacks"<<endl;
		print_bitboard(fileAttacks);
	}*/
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
	return 0;
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
	return 0;
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
		return hangingValues;
	}
}
/*
 * Use the allotted time to run alphaBeta to the highest depth possible in that time.
 * gets all moves, runs alpha-beta on each one of them to depth 0, then sorts them in 
 */
move_t Bitboard::bestMove(time_t calculationTime){
	int depth = 0;
	double eval;
	time_t doneBy = time(0) + calculationTime;
	std::vector<move_t>* movesPtr = allMoves();
	while(time(0) < calculationTime){
		for(int i=0;i<movesPtr->size();i++){
			move((*movesPtr)[i]);
			nodesSearched++;
			eval = alphaBeta(-100,100,depth, doneBy);
			if(eval == TIMEUP){
				takeBack();
				break;
			}
			(*movesPtr)[i].changeEvaluation(eval);
			//cout<<"after inputting to move"<<(*movesPtr)[i].getEvaluation()<<endl;
			takeBack();
		}
		if(eval != TIMEUP){
			depth++;
		}
	}
}
//should get an array with the n best moves.
std::vector<move_t>* Bitboard::nBestMoves(int n, int depth){
	nodesSearched=0;
	int sideToMove=toMove();
	std::vector<move_t>* bestMovesPtr=new std::vector<move_t>();
	std::vector<move_t>* movesPtr = allMoves();
	for(int i=0;i<n;i++){
		(*bestMovesPtr).push_back(nullMove);
		(*bestMovesPtr)[i].changeEvaluation(100*sideToMove*-1);
	}
	for(int i=0;i<movesPtr->size();i++){
		if((*movesPtr)[i]==nullMove){
			continue;
		}
		move((*movesPtr)[i]);
		nodesSearched++;
		double eval = alphaBeta(-100,100,depth);
		(*movesPtr)[i].changeEvaluation(eval);
		if(eval<0){
			cout<<(*movesPtr)[i].tostring()<<(*movesPtr)[i].getEvaluation()<<endl;
			cout<<this;
		}
		takeBack();
	}
	for(int i=0;i<movesPtr->size();i++){
		if ((*movesPtr)[i].getEvaluation()*sideToMove > (*bestMovesPtr)[n-1].getEvaluation()*sideToMove){
			int j;
			for(j=n-1;j>=0&&(*movesPtr)[i].getEvaluation()*sideToMove > (*bestMovesPtr)[j].getEvaluation()*sideToMove;j--){
				if(j!=0){
					(*bestMovesPtr)[j]=(*bestMovesPtr)[j-1];
				}
				else{
					(*bestMovesPtr)[j]=(*movesPtr)[i];
				}
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
double Bitboard::quiesceMini(double alpha, double beta, double tolerance){
	//cout<<this<<endl;
	double standPat = evaluate();
	return standPat;
	if(standPat <= alpha){
		return alpha;
	}
	if(beta > standPat){
		beta = standPat;
	}
	std::vector<move_t>* movesPtr = allCaptures();
	double score;
	if(isDraw()){
		return 0;
	}
	if(isCheckmate()){
		return 15;
	}
	/*if(hangingPieces() < tolerance){
		if(movesPtr->size() == 0){
			return standPat;
		}
	}*/
	if(!isCheck()){ 
		move(nullMove);
		if(movesPtr->size() > 10 && quiesceMaxi(alpha,beta,tolerance) <= alpha){
			takeBack();
			return alpha;
		}
		takeBack();
	}
	int i;
	for(i=0;i<movesPtr->size();i++){
		if((*movesPtr)[i].getPieceTaken() == '_'){
			continue;
		}
		move((*movesPtr)[i]);
		score = quiesceMaxi(alpha,beta,tolerance);
		if(score < -15){
			takeBack();
			return score;
		}
		if(score <= alpha){
			takeBack();
			delete movesPtr;
			//cout<<alpha<<endl;
			return alpha;
		}
		takeBack();
		if(score < beta){
			beta = score;
			if( score <= alpha ){
				break;
			}
		}
	}
	/*if(hangingPieces() > tolerance){
		for(i=i+1;i<movesPtr->size();i++){
			move((*movesPtr)[i]);
			score = quiesceMaxi(alpha,beta,tolerance);
			if(score <= alpha){
				takeBack();
				delete movesPtr;
				return alpha;
			}
			takeBack();
			if(score < beta){
				beta = score;
			}
		}
	}*/
	
	delete movesPtr;
	return beta;
}
double Bitboard::quiesceMaxi(double alpha, double beta, double tolerance){
	//cout<<this<<endl;
	double standPat = evaluate();
	return standPat;
	if(standPat >= beta){
		return beta;
	}
	if(alpha < standPat){
		alpha = standPat;
	}
	std::vector<move_t>* movesPtr = allCaptures();
	int score;
	if(isDraw()){
		return 0;
	}
	if(isCheckmate()){
		return -15;
	}
	/*if(hangingPieces() < tolerance){
		if(movesPtr->size() == 0){
			return standPat;
		}
	}*/
	if(!isCheck()){ 
		move(nullMove);
		if(movesPtr->size() > 10 && quiesceMini(alpha,beta,tolerance) >= beta){
			takeBack();
			return alpha;
		}
		takeBack();
	}
	int i;
	for(i=0;i<movesPtr->size();i++){
		if((*movesPtr)[i].getPieceTaken() == '_'){
			continue;
		}
		move((*movesPtr)[i]);
		score = quiesceMini(alpha,beta,tolerance);
		if(score > 15){
			takeBack();
			return score;
		}
		if(score >= beta){
			takeBack();
			delete movesPtr;
			//cout<<beta<<endl;
			return beta;
		}
		takeBack();
		if(score > alpha){
			alpha = score;
			if( score >= beta ){
				break;
			}
		}
	}
	/*if(hangingPieces() > tolerance){
		for(i=i+1;i<movesPtr->size();i++){
			move((*movesPtr)[i]);
			cout<<(*movesPtr)[i].tostring()<<endl;
			score = quiesceMaxi(alpha,beta,tolerance);
			if(score >= beta){
				takeBack();
				delete movesPtr;
				return beta;
			}
			takeBack();
			if(score > alpha){
				alpha = score;
			}
		}
	}*/
	delete movesPtr;
	return alpha;
}
double Bitboard::quiesce(double alpha, double beta){
	int sideToMove=toMove();
	double tolerance = 150;
	if(sideToMove == -1){
		return quiesceMini(alpha,beta,tolerance);
	}
	return quiesceMaxi(alpha,beta,tolerance);
}
/*
 * Alpha is the highest the evaluation can go, otherwise a black player will reject it
 * Beta is the lowest the evaluation can go, otherwise a white player will reject it
 * depth is the depth of the recursion to go before the quiesce function
 *
 *
 */
double Bitboard::alphaBeta(double alpha, double beta, int depth){
	return alphaBeta(alpha,beta,depth,LONG_MAX);
}
double Bitboard::alphaBeta(double alpha, double beta, int depth,time_t doneBy){
	int sideToMove=toMove();
	double score;
	if(sideToMove == 1){
		score = maxi(alpha,beta,depth,doneBy);
	}
	else{
		score = mini(alpha,beta,depth,doneBy);
	} 
	return score;
}
double Bitboard::mini(double alpha, double beta, int depth){
	return mini(alpha,beta,depth,LONG_MAX);
}
double Bitboard::maxi(double alpha, double beta, int depth){
	return maxi(alpha,beta,depth,LONG_MAX);
}
double Bitboard::mini(double alpha, double beta, int depth, time_t doneBy){
	std::vector<move_t>* movesPtr = allMoves();
	double score;
	if(isDraw()){
		return 0;
	}
	if(isCheckmate()){
		return 15;
	}
	if(depth < 1){
		return quiesce(alpha, beta);
	}
	if(!isCheck()){ 
		move(nullMove);
		if(movesPtr->size() > 10 && maxi(alpha,beta,depth-1) <= alpha){
			takeBack();
			return alpha;
		}
		takeBack();
	}
	for(int myDepth = 0;myDepth < depth-1; myDepth++){
		if(time(0) >= doneBy){
			return TIMEUP;
		}
		for(int i=0;i<movesPtr->size();i++){
			move((*movesPtr)[i]);
			nodesSearched++;
			score = maxi(alpha,beta,myDepth);
			takeBack();
			(*movesPtr)[i].changeEvaluation(score);
		}
		std::sort(movesPtr->begin(),movesPtr->end(),nullMove.evalCompMin);
	}
	
	
	
	for(int i=0;i<movesPtr->size();i++){
		move((*movesPtr)[i]);
		nodesSearched++;
		/*if(nodesSearched%100==0){
			cout<<"searching node "<<nodesSearched<<endl;
			cout<<"depth is "<<depth<<endl;
			cout<<this;
		}*/
		score = maxi(alpha,beta,depth-1);
		if(score < -15){
			takeBack();
			return score;
		}
		if(score <= alpha){
			takeBack();
			delete movesPtr;
			return alpha;
		}
		takeBack();
		if(score < beta){
			beta = score;
		}
	}
	delete movesPtr;
	return beta;
}
double Bitboard::maxi(double alpha, double beta, int depth,time_t doneBy){
	std::vector<move_t>* movesPtr = allMoves();
	double score;
	
	if(isDraw()){
		return 0;
	}
	if(isCheckmate()){
		return -15;
	}
	if(depth < 1){
		return quiesce(alpha, beta);
	}
	if(!isCheck()){ 
		move(nullMove);
		if(movesPtr->size() > 10 && mini(alpha,beta,depth-1) >= beta){
			takeBack();
			return alpha;
		}
		takeBack();
	}
	for(int myDepth = 0;myDepth < depth-1; myDepth++){
		if(time(0) >= doneBy){
			return TIMEUP;
		}
		for(int i=0;i<movesPtr->size();i++){
			move((*movesPtr)[i]);
			nodesSearched++;
			score = mini(alpha,beta,myDepth);
			takeBack();
			(*movesPtr)[i].changeEvaluation(score);
		}
		std::sort(movesPtr->begin(),movesPtr->end(),nullMove.evalCompMax);
	}
	for(int i=0;i<movesPtr->size();i++){
		if((*movesPtr)[i] == 0) continue;
		move((*movesPtr)[i]);
		nodesSearched++;
		/*if(nodesSearched%100==0){
			cout<<"searching node "<<nodesSearched<<endl;
			cout<<"depth is "<<depth<<endl;
			cout<<this;
		}*/
		score = mini(alpha,beta,depth-1);
		if(score > 15){
			takeBack();
			return score;
		}
		if(score >= beta){
			takeBack();
			delete movesPtr;
			return beta;
		}
		takeBack();
		if(score > alpha){
			alpha = score;
		}
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
