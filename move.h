#include <limits.h>
#include <stdint.h>
typedef int32_t move_t;
typedef unsigned uint32_t uint;
namespace move{
	move_t createMove(uint x1, uint y1, uint x2, uint y2, uint moveno, char piece);
	uint x1(move_t move);
	uint y1(move_t move);
	uint x2(move_t move);
	uint y2(move_t move);
	uint moveno(move_t move);
	move_t changePiece(move_t move, char c);
}