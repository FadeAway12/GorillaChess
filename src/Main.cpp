#include <bitset>
#include <iostream>
#include <string>
#include <vector>

#include "UCI.h"
#include "Board.h"
#include "Magic.h"
#include "Move.h"

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

std::string squareToAlg(int sq) { return (char)(sq % 8 + 'A') + std::to_string(8 - (sq / 8));  }

int main() {

	initialize();

	Board::loadFEN("rnbqkbnr/ppp3pp/3pp3/4Pp2/3P4/8/PPP2PPP/RNBQKBNR w KQkq f6 0 4");

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

	/*
	REPRESENTATION OF A MOVE (similar to stockfish's):
	Moves are represented by a 16 bit unsigned int.
	Bits 0-5 represent the destination square (0-63. 0 is A8 and it goes right down to H1. 7 would be H8 and 8 would be A1.
	Bits 6-11 represent the origin square.
	Bits 12-13 represent the promo type if promoting. 0 is queen, 1 is knight, 2-3 can either be black queen/black knight OR bishop/rook,
	depending on whether we care to include those as they're rarely needed
	Bits 14-15 represent a special move (0 - none, 1 - promo, 2 - en passant, 3 - castling)
	*/

	/**
	 * .
	 * std::uint64_t WP, std::uint64_t WN, std::uint64_t WB, std::uint64_t WR, std::uint64_t WQ, std::uint64_t WK,
		std::uint64_t BP, std::uint64_t BN, std::uint64_t BB, std::uint64_t BR, std::uint64_t BQ, std::uint64_t BK,
		std::uint8_t castlingRights, std::uint64_t enPassant
	 * \return 
	 */
	std::vector<std::uint16_t> dwas = Move::whiteMove(Board::WP, Board::WN, Board::WB, Board::WR, Board::WQ, Board::WK, Board::BP, Board::BN, Board::BB, Board::BR, Board::BQ, Board::BK, Board::castlingRights, Board::enPassant);

	std::uint16_t toMask	{ 0b0000000000111111 };
	std::uint16_t fromMask	{ 0b0000111111000000 };
	std::uint16_t promoMask	{ 0b0011000000000000 };
	std::uint16_t specMask	{ 0b1100000000000000 };

	for (int i = 0; i < dwas.size(); i++) {
		std::uint16_t t = dwas[i];
		int to = t & toMask;
		int from = (t & fromMask) >> 6;
		int promoType = (t & promoMask) >> 12;
		int special = (t & specMask) >> 14;
		//std::cout << from << " " << to << std::endl;
		std::cout << squareToAlg(from) << " " << squareToAlg(to) << " " << promoType << " " << special << std::endl;
	}

	return 0;
}