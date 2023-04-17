#include <vector>

#include "Move.h"
#include "Board.h"

namespace Move {

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
	 * Generates all possible white moves
	 * \param WP
	 * \param WN
	 * \param WB
	 * \param WR
	 * \param WQ
	 * \param WK
	 * \param BP
	 * \param BN
	 * \param BB
	 * \param BR
	 * \param BQ
	 * \param BK
	 * \param castlingRights
	 * \param enPassant
	 * \return 
	 */
	std::vector<std::uint16_t> whiteMove(std::uint64_t WP, std::uint64_t WN, std::uint64_t WB, std::uint64_t WR, std::uint64_t WQ, std::uint64_t WK,
		std::uint64_t BP, std::uint64_t BN, std::uint64_t BB, std::uint64_t BR, std::uint64_t BQ, std::uint64_t BK,
		std::uint8_t castlingRights, std::uint64_t enPassant) {

		std::vector<std::uint16_t> moves;
		
		std::uint64_t empty		{ ~(WP | WN | WB | WR | WQ | WK | BP | BN | BB | BR | BQ | BK) };
		std::uint64_t black		{ (BP | BN | BB | BR | BQ | BK) };
		std::uint64_t nWhite	{ ~(WP | WN | WB | WR | WQ | WK) };

		//A pawn can move up if it location shifted up 8 squares isn't occupied by any piece
		std::uint64_t pawnUp	{ WP >> 8 & empty & ~Board::row8};
		std::uint64_t pawnTwoUp	{ ((WP & Board::row2) >> 8 & empty) >> 8 & empty };
		std::uint64_t pawnCapL	{ WP >> 9 & black & ~Board::row8};
		std::uint64_t pawnCapR	{ WP >> 7 & black & ~Board::row8};

		std::uint64_t pawnPromoU{ WP >> 8 & empty & Board::row8 };
		std::uint64_t pawnPromoR{ WP >> 7 & black & Board::row8 };
		std::uint64_t pawnPromoL{ WP >> 9 & black & Board::row8 };

		std::uint64_t enPR		{ WP >> 7 & enPassant};
		std::uint64_t enPL		{ WP >> 9 & enPassant };

		//The knight move variables here represent the direction they move. The first letter means they move 2 spaces in that direction, the second means they move 1 space.
		//So "RU" means 2 right, 1 up
		std::uint64_t knightRU	{ WN >> 6 & ~Board::colA & ~Board::colB & nWhite };
		std::uint64_t knightLU	{ WN >> 10 & ~Board::colG & ~Board::colH & nWhite };
		std::uint64_t knightUR	{ WN >> 15 & ~Board::colA & nWhite };
		std::uint64_t knightUL	{ WN >> 17 & ~Board::colH & nWhite };
		std::uint64_t knightLD	{ WN << 6 & ~Board::colH & ~Board::colG & nWhite };
		std::uint64_t knightRD	{ WN << 10 & ~Board::colA & ~Board::colB & nWhite };
		std::uint64_t knightDL	{ WN << 15 & ~Board::colH & nWhite };
		std::uint64_t knightDR	{ WN << 17 & ~Board::colA & nWhite };

		std::uint64_t kingU		{ WK >> 8 & nWhite };
		std::uint64_t kingR		{ WK << 1 & ~Board::colA & nWhite };
		std::uint64_t kingL		{ WK >> 1 & ~Board::colH & nWhite };
		std::uint64_t kingD		{ WK << 8 & nWhite };
		std::uint64_t kingUR	{ WK >> 7 & ~Board::colA & nWhite };
		std::uint64_t kingUL	{ WK >> 9 & ~Board::colH & nWhite };
		std::uint64_t kingDR	{ WK << 9 & ~Board::colA & nWhite };
		std::uint64_t kingDL	{ WK << 7 & ~Board::colH & nWhite };

		for (int i = 0; i < 64; i++) {

			if (((pawnUp >> i) & 1) == 1) {
				moves.push_back(i + (i + 8 << 6) + (0 << 14));
			}
			if (((pawnTwoUp >> i) & 1) == 1) {
				moves.push_back(i + (i + 16 << 6) + (0 << 14));
			}
			if (((pawnCapL >> i) & 1) == 1) {
				moves.push_back(i + (i + 9 << 6) + (0 << 14));
			}
			if (((pawnCapR >> i) & 1) == 1) {
				moves.push_back(i + (i + 7 << 6) + (0 << 14));
			}
			//For the promos, make it so we add other moves later
			if (((pawnPromoU >> i) & 1) == 1) {
				moves.push_back(i + (i + 8 << 6) + (0 << 12) + (1 << 14));
				moves.push_back(i + (i + 8 << 6) + (1 << 12) + (1 << 14));
			}
			if (((pawnPromoR >> i) & 1) == 1) {
				moves.push_back(i + (i + 7 << 6) + (0 << 12) + (1 << 14));
				moves.push_back(i + (i + 7 << 6) + (1 << 12) + (1 << 14));
			}
			if (((pawnPromoL >> i) & 1) == 1) {
				moves.push_back(i + (i + 9 << 6) + (0 << 12) + (1 << 14));
				moves.push_back(i + (i + 9 << 6) + (1 << 12) + (1 << 14));
			}
			if (((enPR >> i) & 1) == 1) {
				moves.push_back(i + (i + 7 << 6) + (2 << 14));
			}
			if (((enPL >> i) & 1) == 1) {
				moves.push_back(i + (i + 9 << 6) + (2 << 14));
			}
			if (((knightRU >> i) & 1) == 1) {

			}
			if (((knightLU >> i) & 1) == 1) {

			}
			if (((knightUR >> i) & 1) == 1) {

			}
			if (((knightUL >> i) & 1) == 1) {

			}
			if (((knightLD >> i) & 1) == 1) {

			}
			if (((knightRD >> i) & 1) == 1) {

			}
			if (((knightDL >> i) & 1) == 1) {

			}
			if (((knightDR >> i) & 1) == 1) {

			}
			if (((kingU >> i) & 1) == 1) {

			}
			if (((kingR >> i) & 1) == 1) {

			}
			if (((kingL >> i) & 1) == 1) {

			}
			if (((kingD >> i) & 1) == 1) {

			}
			if (((kingUR >> i) & 1) == 1) {

			}
			if (((kingUL >> i) & 1) == 1) {

			}
			if (((kingDR >> i) & 1) == 1) {

			}
			if (((kingDL >> i) & 1) == 1) {

			}
			if (((WR >> i) & 1) == 1) {

			}
			if (((WB >> i) & 1) == 1) {

			}
			if (((WQ >> i) & 1) == 1) {

			}
		}

		return moves;
	}

}
