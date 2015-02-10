#include <limits.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
typedef uint32_t uint;
const int num_piece_types=13;
const char piecemap[num_piece_types]={'P','R','N','B','Q','K','p','r','n','b','q','k','_'};//the characters mapped to numbers using an arr
//a struct to represent a single ply of a chess move.
//fields use bitfields to make the size of this struct the same size as a 4 byte integer.
//ideally pass this around by value.
typedef struct move_t{
private:
	uint indexPiece(char c){
		for(int i=0;i<num_piece_types;i++){
			if(piecemap[i]==c)
				return i;
		}
		return -1;
	}
public:
	unsigned int x1         : 3;
	unsigned int y1         : 3;
	unsigned int x2         : 3;
	unsigned int y2         : 3;
	unsigned int pieceMoved : 4;
	unsigned int pieceTaken : 4;
	bool evaluation_sign    : 1;
	unsigned int evaluation : 11;
	//pieceTaken should be '_' if no piece was taken
	move_t(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, char pieceMoved, char pieceTaken);
	move_t();
	move_t(const int x){
		*(int*)this=x;
	}
	operator int(){
		return *(int*)this;
	}
	move_t& operator= (const int x) {
		*(int*)this=x;
		return *this;
	}
	uint getx1(){return x1;}//the x position of the original square. From 0 to 7
	uint gety1(){return y1;}//the y position of the original square. From 0 to 7
	uint getx2(){return x2;}//the x position of the new square. From 0 to 7
	uint gety2(){return y2;}//the y position of the new square. From 0 to 7
	char getPieceMoved(){return piecemap[pieceMoved];}//the character representing the piece that was moved. rnbqkpRNBQKP
	char getPieceTaken(){return piecemap[pieceTaken];}//the character representing the piece that was taken. 
	//													If none was taken then '_' rnbqkpRNBQKP_
	double getEvaluation(){//Since evaluation must be stored as an int, cast it to a double and divide by 100.
							//this allows for up to a .01 pawn difference between moves, which is more than enough
		int eval=evaluation;
		if(evaluation>30){
			eval = 1000;
		}
		if(evaluation_sign)
			return eval/100.0;
		else 
			return eval/(-100.0);
	}
	void changePieceMoved(char c){pieceMoved=indexPiece(c);};//change the piece moved. 
	//															rnbqkpRNBQKP Don't put anything silly in there or there will be problems
	void changePieceTaken(char c){pieceTaken=indexPiece(c);};//change the piece taken. 
	//															rnbqkpRNBQKP_ Don't put anything silly in there or there will be problems
	void changeEvaluation(double d){//change the evaluation of the move. This is for use by the engine that is traversing the move tree.
		if(d<0){
			d=d*-1;
			evaluation_sign=false;
			evaluation=(int)(d*100);
		}
		else
			evaluation_sign=true;
			evaluation=(int)(d*100);
	}
	bool isCapture(){
		return pieceTaken!=12;
	}
	char* tostring(){
		char* s=new char[100];
		sprintf(s,"%d,%d to %d,%d a %c took a %c\n",this->getx1(),this->gety1(),this->getx2(), this->gety2(), this->getPieceMoved(),this->getPieceTaken());
		return s;
	}
} move_t;

