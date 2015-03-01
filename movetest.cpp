#include "move.h"
#include <stdio.h>
int main(){
	printf("%d\n",sizeof(move_t));
	move_t m(1,1,3,4,'k','n');
	printf("%d,%d to %d,%d a %c took a %c\n",m.getx1(),m.gety1(),m.getx2(), m.gety2(), m.getPieceMoved(),m.getPieceTaken());
	for(float i=-20;i<20;i++){
		m.changeEvaluation(i);
		printf("%f\n",m.getEvaluation());
	}
}