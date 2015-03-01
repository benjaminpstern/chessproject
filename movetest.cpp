#include "move.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
using namespace std;
int testMove1(){
	printf("%d\n",sizeof(move_t));
	move_t m(1,1,3,4,'k','n');
	printf("%d,%d to %d,%d a %c took a %c\n",m.getx1(),m.gety1(),m.getx2(), m.gety2(), m.getPieceMoved(),m.getPieceTaken());
	for(float i=-20;i<20;i++){
		m.changeEvaluation(i);
		printf("%f\n",m.getEvaluation());
	}
}

void testMove2(){
	move_t move;
	move.changeEvaluation(50);
	move.changePieceMoved('p');
	move.changePieceTaken('B');
	cout << move.tostring();
	for(int i=-99;i<100;i++){	
		move.changeEvaluation(i/10.0);
		cout<<i/10.0<<" "<<move.getEvaluation()<<endl;
	}
}
int main(){
	testMove1();
	return 0;
}
