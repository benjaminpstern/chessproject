#include "Bitboard.h"
#include "move.h"
#include <iostream>
#include <string>
using namespace std;
class Chessboard{
private:
	Bitboard** boards;
	move_t* moveHistory;
	void startPosition();
public:
	Chessboard();
	bool occupied(int x, int y);
	bool isLegal(move_t m);
	bool move(move_t m);
	bool takeBack();
	string tostring() const;
	~Chessboard();
};
extern ostream& operator<<(ostream &strm, const Chessboard* board);