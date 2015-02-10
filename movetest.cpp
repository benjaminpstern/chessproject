#include "move.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
using namespace std;
void testMove1(){
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
