chessproject
============
A project that I'm working on, trying to make my own chess engine in C++
A Chessboard is an abstract class, while Bitboards are the actual implementation,
using 64 bit integers as occupancy sets for each type of piece e.g. a white pawn is a type of piece
A move_t is my representation for a move, using 4 bytes(the same amount as an integer) to represent
the x and y coordinates of the starting and ending square, the type of piece moved and taken(if any),
and another 12 bits left over to use for evaluation.
