
all: move bitboard board
move: move.h move.cpp
	g++ -c move.cpp
	g++ -o movetest movetest.cpp move.o
bitboard: Bitboard.h Bitboard.cpp
	g++ -c Bitboard.cpp
board: Chessboard.h Chessboard.cpp Bitboard.o move.o
	g++ -c Chessboard.cpp
	g++ -g -o ChessboardTest ChessboardTest.cpp Chessboard.o Bitboard.o move.o
clean:
	rm -f Chessboard.o ChessboardTest Bitboard.o move.o