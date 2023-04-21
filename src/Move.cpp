#include <vector>
#include <bitset>

#include "Move.h"
#include "Board.h"
#include "Magic.h"

namespace Move {

	/*
	REPRESENTATION OF A MOVE (similar to stockfish's):
	Moves are represented by a 16 bit unsigned int.
	Bits 0-5 represent the destination square (0-63. 0 is A8 and it goes right down to H1. 7 would be H8 and 8 would be A1.
	Bits 6-11 represent the origin square.
	Bits 12-13 represent the promo type if promoting. 0 is queen, 1 is knight, 2-3 can either be black queen/black knight OR bishop/rook,
	depending on whether we care to include those as they're rarely needed
	Bits 14-15 represent a special move (0 - none, 1 - promo, 2 - en passant, 3 - castling)

	"+" to a position here means right and down. "-" to a position means left and up. Remember it goes
	TO and then FROM in the move representation.
	*/

	std::uint16_t toMask{ 0b0000000000111111 };
	std::uint16_t fromMask{ 0b0000111111000000 };
	std::uint16_t promoMask{ 0b0011000000000000 };
	std::uint16_t specMask{ 0b1100000000000000 };

	/**
	 * .
	 * Generates all possible white moves, including psuedo-legal moves
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
				moves.push_back(i + (i + 8 << 6));
			}
			if (((pawnTwoUp >> i) & 1) == 1) {
				moves.push_back(i + (i + 16 << 6));
			}
			if (((pawnCapL >> i) & 1) == 1) {
				moves.push_back(i + (i + 9 << 6));
			}
			if (((pawnCapR >> i) & 1) == 1) {
				moves.push_back(i + (i + 7 << 6));
			}
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
				moves.push_back(i + (i + 6 << 6));
			}
			if (((knightLU >> i) & 1) == 1) {
				moves.push_back(i + (i + 10 << 6));
			}
			if (((knightUR >> i) & 1) == 1) {
				moves.push_back(i + (i + 15 << 6));
			}
			if (((knightUL >> i) & 1) == 1) {
				moves.push_back(i + (i + 17 << 6));
			}
			if (((knightLD >> i) & 1) == 1) {
				moves.push_back(i + (i - 6 << 6));
			}
			if (((knightRD >> i) & 1) == 1) {
				moves.push_back(i + (i - 10 << 6));
			}
			if (((knightDL >> i) & 1) == 1) {
				moves.push_back(i + (i - 15 << 6));
			}
			if (((knightDR >> i) & 1) == 1) {
				moves.push_back(i + (i - 17 << 6));
			}
			if (((kingU >> i) & 1) == 1) {
				moves.push_back(i + (i + 8 << 6));
			}
			if (((kingR >> i) & 1) == 1) {
				moves.push_back(i + (i -1 << 6));
			}
			if (((kingL >> i) & 1) == 1) {
				moves.push_back(i + (i + 1 << 6));
			}
			if (((kingD >> i) & 1) == 1) {
				moves.push_back(i + (i - 8 << 6));
			}
			if (((kingUR >> i) & 1) == 1) {
				moves.push_back(i + (i + 7 << 6));
			}
			if (((kingUL >> i) & 1) == 1) {
				moves.push_back(i + (i + 9 << 6));
			}
			if (((kingDR >> i) & 1) == 1) {
				moves.push_back(i + (i - 9 << 6));
			}
			if (((kingDL >> i) & 1) == 1) {
				moves.push_back(i + (i - 7 << 6));
			}
			if (((WR >> i) & 1) == 1) {
				std::uint64_t poss = Magic::getRookMove(i) & nWhite;
				for (int j = 0; j < 64; j++) {
					if (((poss >> j) & 1) == 1) {
						moves.push_back(j + (i << 6));
					}
				}
			}
			if (((WB >> i) & 1) == 1) {
				std::uint64_t poss = Magic::getBishopMove(i) & nWhite;
				for (int j = 0; j < 64; j++) {
					if (((poss >> j) & 1) == 1) {
						moves.push_back(j + (i << 6));
					}
				}
			}
			if (((WQ >> i) & 1) == 1) {
				std::uint64_t poss = (Magic::getBishopMove(i) | Magic::getRookMove(i)) & nWhite;
				for (int j = 0; j < 64; j++) {
					if (((poss >> j) & 1) == 1) {
						moves.push_back(j + (i << 6));
					}
				}
			}
		}
		//Castling moves
		//For now, castling won't account for attacked squares between king and the rook. This will be added later.
		if ((empty & Board::shortPathW) == Board::shortPathW && castlingRights & Board::whiteKingside) {
			moves.push_back(62 + (60 << 6) + (3 << 14));
		}
		if ((empty & Board::longPathW) == Board::longPathW && castlingRights & Board::whiteQueenside) {
			moves.push_back(58 + (60 << 6) + (3 << 14));
		}

		return moves;
	}

	/**
	 * .
	 * Generates a list of all possible black moves, including pseudo-legal ones.
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
	std::vector<std::uint16_t> blackMove(std::uint64_t WP, std::uint64_t WN, std::uint64_t WB, std::uint64_t WR, std::uint64_t WQ, std::uint64_t WK,
		std::uint64_t BP, std::uint64_t BN, std::uint64_t BB, std::uint64_t BR, std::uint64_t BQ, std::uint64_t BK,
		std::uint8_t castlingRights, std::uint64_t enPassant) {

		std::vector<std::uint16_t> moves;

		std::uint64_t empty{ ~(WP | WN | WB | WR | WQ | WK | BP | BN | BB | BR | BQ | BK) };
		std::uint64_t nBlack{ ~(BP | BN | BB | BR | BQ | BK) };
		std::uint64_t white{ (WP | WN | WB | WR | WQ | WK) };

		//A black pawn can move down if it location shifted down 8 squares isn't occupied by any piece
		std::uint64_t pawnUp{ BP << 8 & empty & ~Board::row1 };
		std::uint64_t pawnTwoUp{ ((BP & Board::row7) << 8 & empty) << 8 & empty };
		std::uint64_t pawnCapL{ BP << 9 & white & ~Board::row1 };
		std::uint64_t pawnCapR{ BP << 7 & white & ~Board::row1 };

		std::uint64_t pawnPromoU{ BP << 8 & empty & Board::row1 };
		std::uint64_t pawnPromoR{ BP << 7 & white & Board::row1 };
		std::uint64_t pawnPromoL{ BP << 9 & white & Board::row1 };

		std::uint64_t enPR{ BP << 7 & enPassant };
		std::uint64_t enPL{ BP << 9 & enPassant };

		//The knight move variables here represent the direction they move. The first letter means they move 2 spaces in that direction, the second means they move 1 space.
		//So "RU" means 2 right, 1 up
		std::uint64_t knightRU{ BN >> 6 & ~Board::colA & ~Board::colB & nBlack };
		std::uint64_t knightLU{ BN >> 10 & ~Board::colG & ~Board::colH & nBlack };
		std::uint64_t knightUR{ BN >> 15 & ~Board::colA & nBlack };
		std::uint64_t knightUL{ BN >> 17 & ~Board::colH & nBlack };
		std::uint64_t knightLD{ BN << 6 & ~Board::colH & ~Board::colG & nBlack };
		std::uint64_t knightRD{ BN << 10 & ~Board::colA & ~Board::colB & nBlack };
		std::uint64_t knightDL{ BN << 15 & ~Board::colH & nBlack };
		std::uint64_t knightDR{ BN << 17 & ~Board::colA & nBlack };

		std::uint64_t kingU{ BK >> 8 & nBlack };
		std::uint64_t kingR{ BK << 1 & ~Board::colA & nBlack };
		std::uint64_t kingL{ BK >> 1 & ~Board::colH & nBlack };
		std::uint64_t kingD{ BK << 8 & nBlack };
		std::uint64_t kingUR{ BK >> 7 & ~Board::colA & nBlack };
		std::uint64_t kingUL{ BK >> 9 & ~Board::colH & nBlack};
		std::uint64_t kingDR{ BK << 9 & ~Board::colA & nBlack };
		std::uint64_t kingDL{ BK << 7 & ~Board::colH & nBlack };

		for (int i = 0; i < 64; i++) {

			if (((pawnUp >> i) & 1) == 1) {
				moves.push_back(i + (i - 8 << 6));
			}
			if (((pawnTwoUp >> i) & 1) == 1) {
				moves.push_back(i + (i - 16 << 6));
			}
			if (((pawnCapL >> i) & 1) == 1) {
				moves.push_back(i + (i - 9 << 6));
			}
			if (((pawnCapR >> i) & 1) == 1) {
				moves.push_back(i + (i - 7 << 6));
			}
			if (((pawnPromoU >> i) & 1) == 1) {
				moves.push_back(i + (i - 8 << 6) + (0 << 12) + (1 << 14));
				moves.push_back(i + (i - 8 << 6) + (1 << 12) + (1 << 14));
			}
			if (((pawnPromoR >> i) & 1) == 1) {
				moves.push_back(i + (i - 9 << 6) + (0 << 12) + (1 << 14));
				moves.push_back(i + (i - 9 << 6) + (1 << 12) + (1 << 14));
			}
			if (((pawnPromoL >> i) & 1) == 1) {
				moves.push_back(i + (i - 7 << 6) + (0 << 12) + (1 << 14));
				moves.push_back(i + (i - 7 << 6) + (1 << 12) + (1 << 14));
			}
			if (((enPR >> i) & 1) == 1) {
				moves.push_back(i + (i - 9 << 6) + (2 << 14));
			}
			if (((enPL >> i) & 1) == 1) {
				moves.push_back(i + (i - 7 << 6) + (2 << 14));
			}
			if (((knightRU >> i) & 1) == 1) {
				moves.push_back(i + (i + 6 << 6));
			}
			if (((knightLU >> i) & 1) == 1) {
				moves.push_back(i + (i + 10 << 6));
			}
			if (((knightUR >> i) & 1) == 1) {
				moves.push_back(i + (i + 15 << 6));
			}
			if (((knightUL >> i) & 1) == 1) {
				moves.push_back(i + (i + 17 << 6));
			}
			if (((knightLD >> i) & 1) == 1) {
				moves.push_back(i + (i - 6 << 6));
			}
			if (((knightRD >> i) & 1) == 1) {
				moves.push_back(i + (i - 10 << 6));
			}
			if (((knightDL >> i) & 1) == 1) {
				moves.push_back(i + (i - 15 << 6));
			}
			if (((knightDR >> i) & 1) == 1) {
				moves.push_back(i + (i - 17 << 6));
			}
			if (((kingU >> i) & 1) == 1) {
				moves.push_back(i + (i + 8 << 6));
			}
			if (((kingR >> i) & 1) == 1) {
				moves.push_back(i + (i - 1 << 6));
			}
			if (((kingL >> i) & 1) == 1) {
				moves.push_back(i + (i + 1 << 6));
			}
			if (((kingD >> i) & 1) == 1) {
				moves.push_back(i + (i - 8 << 6));
			}
			if (((kingUR >> i) & 1) == 1) {
				moves.push_back(i + (i + 7 << 6));
			}
			if (((kingUL >> i) & 1) == 1) {
				moves.push_back(i + (i + 9 << 6));
			}
			if (((kingDR >> i) & 1) == 1) {
				moves.push_back(i + (i - 9 << 6));
			}
			if (((kingDL >> i) & 1) == 1) {
				moves.push_back(i + (i - 7 << 6));
			}
			if (((BR >> i) & 1) == 1) {
				std::uint64_t poss = Magic::getRookMove(i) & nBlack;
				for (int j = 0; j < 64; j++) {
					if (((poss >> j) & 1) == 1) {
						moves.push_back(j + (i << 6));
					}
				}
			}
			if (((BB >> i) & 1) == 1) {
				std::uint64_t poss = Magic::getBishopMove(i) & nBlack;
				for (int j = 0; j < 64; j++) {
					if (((poss >> j) & 1) == 1) {
						moves.push_back(j + (i << 6));
					}
				}
			}
			if (((BQ >> i) & 1) == 1) {
				std::uint64_t poss = (Magic::getBishopMove(i) | Magic::getRookMove(i)) & nBlack;
				for (int j = 0; j < 64; j++) {
					if (((poss >> j) & 1) == 1) {
						moves.push_back(j + (i << 6));
					}
				}
			}
		}
		//Castling moves
		//For now, castling won't account for attacked squares between king and the rook. This will be added later.
		if ((empty & Board::shortPathB) == Board::shortPathB && castlingRights & Board::blackKingside) {
			moves.push_back(62 + (60 << 6) + (3 << 14));
		}
		if ((empty & Board::longPathB) == Board::longPathB && castlingRights & Board::blackQueenside) {
			moves.push_back(58 + (60 << 6) + (3 << 14));
		}

		return moves;
	}

}
