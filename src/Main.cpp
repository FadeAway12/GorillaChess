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
	Board::arrOfSquares[0] = 0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'00000001;
	for (int i = 0; i < 64; i++) {
		Magic::blockerBoardRook(i);
		Magic::blockerBoardBishop(i);
		Board::arrOfSquares[i] = Board::arrOfSquares[0] << i;
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

	Board::loadFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

	Board::printBoard();

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
	std::vector<std::uint16_t> dwas = Move::blackMove(Board::WP, Board::WN, Board::WB, Board::WR, Board::WQ, Board::WK, Board::BP, Board::BN, Board::BB, Board::BR, Board::BQ, Board::BK, Board::castlingRights, Board::enPassant);

	for (int i = 0; i < dwas.size(); i++) {
		std::uint16_t t = dwas[i];
		int to = t & Move::toMask;
		int from = (t & Move::fromMask) >> 6;
		int promoType = (t & Move::promoMask) >> 12;
		int special = (t & Move::specMask) >> 14;
		//std::cout << from << " " << to << std::endl;
		std::cout << squareToAlg(from) << " " << squareToAlg(to) << " " << promoType << " " << special << std::endl;
	}

	return 0;
}