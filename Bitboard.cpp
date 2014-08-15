
#include "Bitboard.h"
#define num_piece_types 12
#define numMoves 1024
const char chars[num_piece_types]={'P','R','N','B','Q','K','p','r','n','b','q','k'};
const int pieceValues[num_piece_types]={1,5,3,3,9,40,-1,-5,-3,-3,-9,-40};
const move_t nullMove;
Bitboard::Bitboard(){
	bitbrds=new uint64_t [num_piece_types];
	for(int i=0;i<num_piece_types;i++){
		bitbrds[i]=0;
	}
	nodesSearched=0;
	startPosition();
	moveHistory=(move_t*)malloc(sizeof(move_t)*2048);
	plyNo=0;
	moveHistory[plyNo]=nullMove;
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
//gives all the squares the pieces are attacking on board pieceIndex
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
//gives all the squares the pieces are attacking on brd, using the type of piece referred to by pieceIndex
uint64_t Bitboard::pieceAttacks(int pieceIndex, uint64_t brd){
	switch(pieceIndex){
		case 0:
			return pawnAttacks(brd,0);
		case 1:
			return rookAttacks(brd,0);
		case 2:
			return knightAttacks(brd,0);
		case 3:
			return bishopAttacks(brd,0);
		case 4:
			return queenAttacks(brd,0);
		case 5:
			return kingAttacks(brd,0);
		case 6:
			return pawnAttacks(brd,1);
		case 7:
			return rookAttacks(brd,1);
		case 8:
			return knightAttacks(brd,1);
		case 9:
			return bishopAttacks(brd,1);
		case 10:
			return queenAttacks(brd,1);
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
	return ((bitbrds[5]&enemyPieceAttacks[0])|(bitbrds[11]&enemyPieceAttacks[1]))!=0;
}
bool Bitboard::isCheckmate(){
	//if both of the kings can move somewhere, it's not checkmate.
	//this relies on the kingAttacks function to generate taboo squares correctly.
	if(pieceAttacks(5)!=0&&pieceAttacks(11)!=0){
		return false;
	}
	int cpn=0;//the checking pieces number, or the number of pieces that have been added to checkingPieces
	int checkingPiece;
	//searching the white pieces to see if any of them can attack the king
	for(int i=0;i<5;i++){
		if (pieceAttacks(i)&bitbrds[11]){
			checkingPiece=i;
			cpn++;
		}
		if(cpn==2){
			return true;
		}
	}
	if(cpn==0){
		for(int i=6;i<11;i++){
			if (pieceAttacks(i)&bitbrds[5]){
				checkingPiece=i;
				cpn++;
			}
			if(cpn==2){
				return true;//if it's double check, we've already made sure the king can't move anywhere
				//so it must be checkmate
			}
		}
		if(cpn==0){
			return false;
		}
		//at this point white must be in check.
		
	}
	else{//white must be checking black
		
	}

}
/*
 * pointer to null-terminated array of all legal moves in the position
 * the captures will be first and the rest of the moves will come afterward
 */
move_t* Bitboard::allMoves(){
	int sideToMove=toMove();
	move_t* moves = new move_t[250];
	move_t* nonCaptures = new move_t[250];
	int moveno=0;
	int nonCaptureNo=0;
	int i=0;
	if(sideToMove<0) i=6;
	int max=i+6;
	for(;i<max;i++){
		move_t* pieceMoves=allMoves(i);
		int j;
		for(j=0;pieceMoves[j].getPieceTaken()!='_'&&pieceMoves[j]!=0;j++){
			//cout<<j<<endl;
			moves[moveno]=pieceMoves[j];
			moveno++;
		}
		for(;pieceMoves[j]!=0;j++){
			nonCaptures[nonCaptureNo]=pieceMoves[j];
			nonCaptureNo++;
		}
		delete [] pieceMoves;
	}
	for(int j=0;j<nonCaptureNo;j++){
		moves[moveno]=nonCaptures[j];
		moveno++;
	}
	delete [] nonCaptures;
	moves[moveno]=0;
	return moves;
}
//returns the moves of a piece, with the captures first
move_t* Bitboard::allMoves(int pieceIndex){
	uint64_t first=firstPiece(bitbrds[pieceIndex]);
	uint64_t rest=restPieces(bitbrds[pieceIndex]);
	uint64_t own = ownPieces(pieceIndex/6);
	uint64_t enemy = enemyPieces(pieceIndex/6);
	move_t* moves=new move_t[250];
	move_t* nonCaptures = new move_t[250];
	int moveno=0;
	int nonCaptureNo=0;
	while(first){
		uint64_t pieceMoves=pieceAttacks(pieceIndex,first)&~own;
		uint64_t pieceCaptures = enemy&pieceMoves;
		pieceMoves &= ~enemy;
		while(pieceCaptures){
			uint64_t firstMove=firstPiece(pieceCaptures);
			int x=xValue(firstMove);
			int y=yValue(firstMove);
			moves[moveno]=move_t(xValue(first),yValue(first),x,y,chars[pieceIndex],piece(x,y));
			pieceCaptures=restPieces(pieceCaptures);
			moveno++;
		}
		while(pieceMoves){
			uint64_t firstMove=firstPiece(pieceMoves);
			int x=xValue(firstMove);
			int y=yValue(firstMove);
			nonCaptures[nonCaptureNo]=move_t(xValue(first),yValue(first),x,y,chars[pieceIndex],'_');
			pieceMoves=restPieces(pieceMoves);
			nonCaptureNo++;
		}
		first=firstPiece(rest);
		rest=restPieces(rest);
	}
	for(int i=0;i<nonCaptureNo;i++){
		moves[moveno]=nonCaptures[i];
		moveno++;
	}
	delete [] nonCaptures;
	moves[moveno]=0;
	return moves;
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
	moveHistory[plyNo]=0;
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

	//cout<<plyNo<<endl;
	moveHistory[plyNo]=0;
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
	uint64_t occ=occupancySet();
	//int moveSquares[4]={1,7,8,9};
	//for(int i=0;i<4;i++){
	newSquares|=(brd<<1)&notFirstRank;
	newSquares|=(brd>>1)&notLastRank;
	newSquares|=(brd<<7)&notLastRank;
	newSquares|=(brd>>7)&notFirstRank;
	newSquares|=(brd<<8);
	newSquares|=(brd>>8);
	newSquares|=(brd<<9)&notFirstRank;
	newSquares|=(brd>>9)&notLastRank;
	//}
	//cout<<1<<endl;
	//newSquares&=~(ownPieces(blackOrWhite));
	for(int i=otherPieces(blackOrWhite);i<otherPieces(blackOrWhite)+5;i++){//to exclude the enemy king
		taboo|=pieceAttacks(i);
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
/*
U64 diagonalAttacks(U64 occ, int sqOfSlider) {
   U64 forward, reverse, slider, lineMask;
 
   lineMask = diagonalMaskEx[sqOfSlider]; // excludes square of slider
   slider   = singleBitboard[sqOfSlider]; // single bit 1 << sq, 2^sq
 
   forward  = occ & lineMask; // also performs the first subtraction by clearing the s in o
   reverse  = byteswap( forward ); // o'-s'
   forward -=         ( slider  ); // o -2s
   reverse -= byteswap( slider  ); // o'-2s'
   forward ^= byteswap( reverse );
   return forward & lineMask;      // mask the line again
}
*/
uint64_t Bitboard::bishopAttacks(uint64_t brd, int blackOrWhite){
	uint64_t newSquares = 0;
	uint64_t forward;
	uint64_t rev;
	uint64_t lineMask1;
	uint64_t lineMask2;
	uint64_t bishop;
	uint64_t occ = occupancySet();
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
		newSquares |= (forward & lineMask2);
	}
	return newSquares;
	
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
move_t* Bitboard::nBestMoves(int n){
	nodesSearched=0;
	int sideToMove=toMove();
	move_t* bestMoves=new move_t[n];
	for(int i=0;i<n;i++){
		bestMoves[i].changeEvaluation(40*sideToMove*-1);
		//cout<<bestMoves[i].getEvaluation()*sideToMove<<endl;
	}
	move_t* moves = allMoves();
	for(int i=0;moves[i]!=0;i++){
		move(moves[i]);
		nodesSearched++;
		//cout<<this;
		move_t m=moves[i];
		moves[i].changeEvaluation(evaluate(2));
		takeBack();
		//cout<<this;
	}
	for(int i=0;moves[i];i++){
		//cout<<moves[i].getEvaluation()*sideToMove<<" "<<bestMoves[n-1].getEvaluation()*sideToMove<<endl;
		if (moves[i].getEvaluation()*sideToMove > bestMoves[n-1].getEvaluation()*sideToMove){
			int j;
			for(j=n-1;j>=0&&moves[i].getEvaluation()*sideToMove > bestMoves[j].getEvaluation()*sideToMove;j--){
				if(j!=0){
					bestMoves[j]=bestMoves[j-1];
				}
				else{
					bestMoves[j]=moves[i];
				}
				//cout<<j<<endl;
			}
		}
	}
	delete [] moves;
	cout<<"searched "<<nodesSearched<<" nodes"<<endl;
	cout<<"evaluation is now "<<bestMoves[0].getEvaluation()<<endl;
	return bestMoves;
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
	//cout<<this<<endl<<depth<<endl;
	double eval=evaluate();
	int hanging=hangingPieces();
	if(depth<1){
		return eval;
	}
	int sideToMove=toMove();
	double bestEvaluation=40*sideToMove*-1;
	move_t* moves = allMoves();
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
		delete [] moves;
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
	delete [] moves;
	return bestEvaluation;
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