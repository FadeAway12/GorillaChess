#pragma once

#include <iostream>

namespace Move {

	std::vector<std::uint16_t> whiteMove(std::uint64_t WP, std::uint64_t WN, std::uint64_t WB, std::uint64_t WR, std::uint64_t WQ, std::uint64_t WK,
		std::uint64_t BP, std::uint64_t BN, std::uint64_t BB, std::uint64_t BR, std::uint64_t BQ, std::uint64_t BK,
		std::uint8_t castlingRights, std::uint64_t enPassant);

}
