#include <bitset>
#include <iostream>

#include "UCI.h"
#include "Board.h"
#include "Magic.h"

/**
 * .
 * Initializes all the move list maps.
 */
void initialize() {
	for (int i = 0; i < 64; i++) {
		Magic::blockerBoardRook(i);
		Magic::blockerBoardBishop(i);
	}
}

void printBitBoard(const std::uint64_t& b, std::ostream& os) {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			os << (((b >> (i * 8) + j) & 1) == 1) << " ";
		}
		os << '\n';
	}
	os << std::endl;
}

int main() {

	initialize();

	Board::loadFEN("3k3r/pp2rp1p/3p4/2p1P1B1/1b6/3K3Q/PP3P2/n7 w - c6 0 23");

	Board::printBoard();

	for (int i = 0; i < 64; i++) {
		if (((Board::WR >> i) & 1) == 1) {
			std::cout << "WHITE ROOK " << std::endl;
			std::uint64_t moves = Magic::getRookMove(i);
			printBitBoard(moves, std::cout);
		}
		else if (((Board::WB >> i) & 1) == 1) {
			std::cout << "WHITE BISHOP " << std::endl;
			std::uint64_t moves = Magic::getBishopMove(i);
			printBitBoard(moves, std::cout);
		}
		else if (((Board::WQ >> i) & 1) == 1) {
			std::cout << "WHITE QUEEN " << std::endl;
			std::uint64_t moves1 = Magic::getBishopMove(i);
			std::uint64_t moves2 = Magic::getRookMove(i);
			printBitBoard(moves1 | moves2, std::cout);
		}
		else if (((Board::BR >> i) & 1) == 1) {
			std::cout << "BLACK ROOK " << std::endl;
			std::uint64_t moves = Magic::getRookMove(i);
			printBitBoard(moves, std::cout);
		}
		else if (((Board::BB >> i) & 1) == 1) {
			std::cout << "BLACK BISHOP " << std::endl;
			std::uint64_t moves = Magic::getBishopMove(i);
			printBitBoard(moves, std::cout);
		}
		else if (((Board::BQ >> i) & 1) == 1) {
			std::cout << "BLACK QUEEN " << std::endl;
			std::uint64_t moves1 = Magic::getBishopMove(i);
			std::uint64_t moves2 = Magic::getRookMove(i);
			printBitBoard(moves1 | moves2, std::cout);
		}
	}

	return 0;
}