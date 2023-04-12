#include <unordered_map>
#include <bitset>
#include <fstream>

#include "Board.h"

namespace Magic {

	/*
	* TESTING THINGS
	*/
	std::ofstream ost("WritingTests.txt");
	int a{};

	int findNextBit(const std::uint64_t& b);
	void blockerBoardRook(int index, std::uint64_t blockerBoard, std::uint64_t blockerMask);
	void rookBlockerToMove(int index, std::uint64_t blockerBoard);
	void printBitBoard(const std::uint64_t& b, std::ostream& os);

	/*
	* Used to hash the moves available for the rook by blocker boards.
	* magicRook & magicBishop store the magic numbers for each square.
	*/

	std::uint64_t** rookMoves[64];
	int magicRook[64];

	std::uint64_t** bishopMoves[64];
	int magicBishop[64];
	
	void initialize() {

	}

	int getMagic() {
		int max = pow(2, 32);
		int min = 0;
		srand(time(NULL)); // Seed the time
		int finalNum = rand() % (max - min + 1) + min; // Generate the number, assign to variable.
		return finalNum;
	}

	/**
	 * .
	 * Creates a bitboard of all the candidate blockers for a rook from a certain square.
	 * It checks the column and row the square is on. From there, it creates a bitboard by or'ing the row and column it's on, then not'ing the edge rows and columns.
	 * This creates all candidate blockers.
	 * \param square
	 * \return
	 */
	std::uint64_t blockerMaskRook(int square) {

		std::uint64_t sq = pow(2, square);
		std::uint64_t col{ 0 }, row{ 0 };
		switch (square / 8) {
		case 7: row = Board::row1; break;
		case 6: row = Board::row2; break;
		case 5: row = Board::row3; break;
		case 4: row = Board::row4; break;
		case 3: row = Board::row5; break;
		case 2: row = Board::row6; break;
		case 1: row = Board::row7; break;
		case 0: row = Board::row8; break;
		}
		switch (square % 8) {
		case 0: col = Board::colA; break;
		case 1: col = Board::colB; break;
		case 2: col = Board::colC; break;
		case 3: col = Board::colD; break;
		case 4: col = Board::colE; break;
		case 5: col = Board::colF; break;
		case 6: col = Board::colG; break;
		case 7: col = Board::colH; break;
		}

		row &= ~(Board::colA | Board::colH);
		col &= ~(Board::row1 | Board::row8);
		return (row | col) & ~sq;
	}

	/**
	 * .
	 * Creates blocker boards of all permutations (2^numBits) recursively.
	 * \param index
	 * \param blockerMask
	 */
	void blockerBoardRook(int index) {

		//a = 0;

		std::uint64_t blockerMask = blockerMaskRook(index);
		int count = std::popcount(blockerMask);
		int magic = getMagic();
		magicRook[index] = magic;
		blockerBoardRook(index, 0, blockerMask);
	}

	/**
	 * .
	 * Is recursively called. Calls rookBlockerToMove to generate the moves then hash them for easy access during legal moveGen.
	 * Essentially, every bit in the blockerMask can either be included in the blockerBoard or not. It then calls itself recursively by doing this
	 * and not'ing the next bit in the blocker board. This provides all possibilities. Once the amount of bits in the mask is 0, all bits have been used.
	 * From here, it converts it to a move and hashes it for later use.
	 * \param index
	 * \param blockerBoard
	 * \param blockerMask
	 */
	void blockerBoardRook(int index, std::uint64_t blockerBoard, std::uint64_t blockerMask) {
		int count = std::popcount(blockerMask);
		if (count == 0) {
			rookBlockerToMove(index, blockerBoard);
			return;
		}
		int ind = findNextBit(blockerMask);
		std::uint64_t nextBit = (std::uint64_t)pow(2, ind);
		blockerMask &= ~(nextBit);
		blockerBoardRook(index, blockerBoard | nextBit, blockerMask);
		blockerBoardRook(index, blockerBoard, blockerMask);

	}

	/**
	 * .
	 * Converts a blockerboard and an index (where the rook is on) to a move bitboard.
	 * It simply uses a while loop to repeatedly shift it upwards, downwards, leftwards or rightwards.
	 * If it encounters a blockerboard it or's it and then quits the while loop.
	 * \param index
	 * \param blockerBoard
	 */
	void rookBlockerToMove(int index, std::uint64_t blockerBoard) {
		std::uint64_t pos = pow(2, index);
		std::uint64_t moves{};
		std::uint64_t U{ pos }, R{ pos }, L{ pos }, D{ pos };
		
		while (U) {
			U = U >> 8;
			moves |= U;
			U &= ~blockerBoard;
		}
		while (D) {
			D = D << 8;
			moves |= D;
			D &= ~blockerBoard;
		}
		while (L) {
			L = L >> 1 & ~Board::colH;
			moves |= L;
			L &= ~(blockerBoard);
		}
		while (R) {
			R = R << 1 & ~Board::colA;
			moves |= R;
			R &= ~(blockerBoard);
		}
		moves &= ~pos;
		//ost << a << std::endl << std::endl; a++;
		//printBitBoard(blockerBoard, ost);
		//printBitBoard(moves, ost);
	}

	/**
	 * .
	 * Algorithm that counts the amount of "1s" in a bitboard. This isn't needed and we can just use std::popcount(i) if in C++20.
	 * \param u
	 * \return
	 */
	int onesInBB(std::uint64_t u) {
		std::uint64_t uCount;
		uCount = u - ((u >> 1) & 033333333333) - ((u >> 2) & 011111111111);
		return ((uCount + (uCount >> 3)) & 030707070707) % 63;
	}

	/**
	 * .
	 * Self-explanatory. Used to find the next "1" in a bitboard.
	 * \param b
	 * \return
	 */
	int findNextBit(const std::uint64_t& b) {
		for (int i = 0; i < 64; i++) {
			if (((b >> i) & 1) == 1) return i;
		}
		return -1;
	}

	/**
	 * .
	 * Prints out bitboard as 1s and 0s.
	 * \param b
	 * \param os
	 */
	void printBitBoard(const std::uint64_t& b, std::ostream& os) {
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				os << (((b >> (i * 8) + j) & 1) == 1) << " ";
			}
			os << '\n';
		}
		os << std::endl;
	}




}

int main() {

	//std::uint64_t bb = Magic::blockerMaskRook(33);
	
	//Magic::blockerBoardRook(33);
	
	for (int i = 0; i < 64; i++) {

		Magic::ost << (char)(i / 8 + 'A') << (i % 8)+1 << " ";

		Magic::blockerBoardRook(i);
	}

	return 0;
}

/* LEAVING THIS HERE FOR NOW AS I IMPLEMENT ROOK MAGIC BITBOARDS FIRST
*
	std::uint64_t blockerMaskBishop(int square) {
		std::uint64_t sq = pow(2, square);
		int n{ 1 };
		std::uint64_t blocker{};
		std::uint64_t ur = sq, ul = sq, dr = sq, dl = sq;
		while (ur || ul || dr || dl) {
			ur = ur >> 7 & ~Board::colA;
			ul = ul >> 9 & ~Board::colH;
			dr = dr << 9 & ~Board::colA;
			dl = dl << 7 & ~Board::colH;
			blocker |= (ur) | (ul);
			blocker |= (dr) | (dl);
		}
		blocker &= ~(sq | Board::row8 | Board::row1 | Board::colA | Board::colH);
		return blocker;
	}
	*/