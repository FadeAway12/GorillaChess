#pragma once
#include <iostream>

namespace Magic {

	extern void blockerBoardBishop(int index);
	extern void blockerBoardRook(int index);
	extern std::uint64_t getRookMove(int sq);
	extern std::uint64_t getBishopMove(int sq);

}
