#include <unordered_map>

namespace Magic {

	void initialize() {
		
	}

	/**
	 * .
	 * Algorithm that counts the amount of "1s" in a bitboard.
	 * \param u
	 * \return 
	 */
	int onesInBB(std::uint64_t u) {
		std::uint64_t uCount;
		uCount = u - ((u >> 1) & 033333333333) - ((u >> 2) & 011111111111);
		return ((uCount + (uCount >> 3)) & 030707070707) % 63;
	}

}
