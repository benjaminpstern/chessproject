#include <limits.h>
#include <stdint.h>
typedef unsigned int uint;

const char piecemap[12]={'P','R','N','B','Q','K','p','r','n','b','q','k'};
typedef struct move_t{
private:
	uint indexPiece(char c){
		for(int i=0;i<12;i++){
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
	move_t(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, char pieceMoved, char pieceTaken);
	uint getx1(){return x1;}
	uint gety1(){return y1;}
	uint getx2(){return x2;}
	uint gety2(){return y2;}
	char getpieceMoved(){return piecemap[pieceMoved];}
	char getpieceTaken(){return piecemap[pieceTaken];}
	double getevaluation(){
		if(evaluation_sign)
			return (double)evaluation/100;
		else 
			return (double)evaluation/100*-1;}
	void changePieceMoved(char c){pieceMoved=indexPiece(c);};
	void changePieceTaken(char c){pieceTaken=indexPiece(c);};
	void changeEvaluation(double d){
		if(d<0){
			evaluation_sign=false;
			evaluation=(int)d*-100;
		}
		else
			evaluation=(int)d*100;
	}
} move_t;

