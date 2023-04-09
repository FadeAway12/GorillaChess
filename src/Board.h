#pragma once

#include <iostream>

namespace Board {

	extern std::uint64_t WP;
	extern std::uint64_t WN;
	extern std::uint64_t WB;
	extern std::uint64_t WR;
	extern std::uint64_t WQ;
	extern std::uint64_t WK;

	extern std::uint64_t BP;
	extern std::uint64_t BN;
	extern std::uint64_t BB;
	extern std::uint64_t BR;
	extern std::uint64_t BQ;
	extern std::uint64_t BK;

	extern std::uint64_t enPassant;

	extern std::uint8_t castlingRights;
	extern std::uint8_t whiteKingside;
	extern std::uint8_t whiteQueenside;
	extern std::uint8_t blackKingside;
	extern std::uint8_t blackQueenside;

	extern std::uint16_t moveNum;
	extern std::uint8_t fiftyDraw;

	extern bool whiteTurn;

	extern void loadFEN(std::string FEN);

	extern void printBoard();

}
