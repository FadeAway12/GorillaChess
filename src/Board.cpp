#include <iostream>
#include <sstream>
#include <bitset>

#include "Board.h"

/*
Contains the representation of the board, including position, en passant, castling, 50-move draw, and moveNum.
Also has the loadFEN(string) method (for loading FEN into the representation). The printBoard() method simply
prints a representation of the board to the console.
*/

namespace Board {

	//Represents position of each type of piece. Reads from (A8) right down (H1). 
	//A8 is rightmost when printed, H1 is leftmost when printed.

	std::uint64_t WP;
	std::uint64_t WN;
	std::uint64_t WB;
	std::uint64_t WR;
	std::uint64_t WQ;
	std::uint64_t WK;

	std::uint64_t BP;
	std::uint64_t BN;
	std::uint64_t BB;
	std::uint64_t BR;
	std::uint64_t BQ;
	std::uint64_t BK;

	//Represents an available square which can be captured en passant

	std::uint64_t enPassant;

	//Ignore the left 4 bits. Use the 4 helper bit flags to check or not castling rights.

	std::uint8_t castlingRights;
	std::uint8_t whiteKingside{  0b000000001 };
	std::uint8_t whiteQueenside{ 0b000000010 };
	std::uint8_t blackKingside{  0b000000100 };
	std::uint8_t blackQueenside{ 0b000001000 };

	//Movenum measures the amount of full moves since the game's start, starting at 1 and incremented at
	//the end of each black move. Fiftydraw represents the number of half moves since a capture or pawn advance,
	//and is incremented at every turn. It starts at 0.

	std::uint16_t moveNum;
	std::uint8_t fiftyDraw;

	//Represents whether it is white or black to play

	bool whiteTurn;

	/**
	 * .
	 * Parses the string and sets the board representation to
	reflect the FEN parameter.
	 * \param FEN
	 */
	void loadFEN(std::string FEN) {

		/*
		We use an istream to read different spaced strings in the FEN string into different values.
		This allows us to more easily set their values without messing with indexes.
		*/

		std::string position;
		char turn{};
		std::string castling;
		std::string enP;
		std::uint8_t fifDraw{};
		std::uint16_t moveCount{};

		std::istringstream reader{ FEN };
		
		reader >> position >> turn >> castling >> enP >> fifDraw >> moveCount;

		/*
		* Setting the piece bitboards to the proper values. Goes through each row and finds a substring from the start
		* to the location of the first slash (which separates rows). It goes through each column, and if it has a number,
		* it increments j by that number. Otherwise, if its a piece, it sets that bit on the piece's bitboard.
		* Lastly, it sets the position substring from the slash it found plus one to the size of the position string.
		*/

		for (int i = 0; i < 8; i++) {

			int charSlash = position.find('/');

			std::string row = position.substr(0, charSlash);

			int count{ 0 };

			for (int j = 0; j < 8; j++) {

				switch (row[count]) {
				
				case 'r': BR |= ((std::uint64_t)pow(2, i * 8 + j)); break;
				case 'n': BN |= ((std::uint64_t)pow(2, i * 8 + j)); break;
				case 'p': BP |= ((std::uint64_t)pow(2, i * 8 + j)); break;
				case 'q': BQ |= ((std::uint64_t)pow(2, i * 8 + j)); break;
				case 'k': BK |= ((std::uint64_t)pow(2, i * 8 + j)); break;
				case 'b': BB |= ((std::uint64_t)pow(2, i * 8 + j)); break;
				case 'R': WR |= ((std::uint64_t)pow(2, i * 8 + j)); break;
				case 'N': WN |= ((std::uint64_t)pow(2, i * 8 + j)); break;
				case 'P': WP |= ((std::uint64_t)pow(2, i * 8 + j)); break;
				case 'Q': WQ |= ((std::uint64_t)pow(2, i * 8 + j)); break;
				case 'K': WK |= ((std::uint64_t)pow(2, i * 8 + j)); break;
				case 'B': WB |= ((std::uint64_t)pow(2, i * 8 + j)); break;
				default: j += (row[count] - '0') - 1;

				}

				count++;

			}

			position = position.substr(charSlash + 1, position.size());

		}

		/*
		* If the turn variable is a 'w', we set the turn to white. Otherwise, it has to be black's turn.
		*/

		if (turn == 'w') whiteTurn = true;
		else whiteTurn = false;

		/*
		* Goes through every character in the enP string. If there's a K, white can castle kingside. A Q, white can
		* castle queenside, and so on.
		*/

		for (int i = 0; i < castling.size(); i++) {

			switch (castling[i]) {
			case 'K': castlingRights |= whiteKingside; break;
			case 'Q': castlingRights |= whiteQueenside; break;
			case 'k': castlingRights |= blackKingside; break;
			case 'q': castlingRights |= blackQueenside; break;
			}

		}

		/*
		* Generates available en passant square by finding the column and row and setting it on the en passant bitboard.
		*/

		if (enP != "-") {

			int col = enP[0] - 'a';
			int row = 8-(enP[1] - '0');
			enPassant = pow(2, row * 8 + col);

		}

		/*
		* Sets move number and moves since capture or pawn push.
		*/
		
		fiftyDraw = fifDraw;
		
		moveNum = moveCount;

	}
	
	/**
	 * .
	 * Prints the representation of the board, including castling rights, en passant squares, and the turn.
	 */
	void printBoard() {

		/*
		Checks the state of the board and prints out relevant info.
		*/
		
		if (whiteTurn) std::cout << "White to play\n";
		else std::cout << "Black to play\n";

		if (castlingRights & whiteKingside) std::cout << "White can castle kingside\n";
		if (castlingRights & whiteQueenside) std::cout << "White can castle queenside\n";
		if (castlingRights & blackKingside) std::cout << "Black can castle kingside\n";
		if (castlingRights & blackQueenside) std::cout << "Black can castle queenside\n";

		std::cout << "EN PASSANT: " << std::bitset<64>(enPassant) << std::endl;

		std::cout << "moveNum: " << moveNum << std::endl;

		std::cout << "Since cap or pawn: " << fiftyDraw << std::endl;

		/*
		Goes through the bitboards and prints out if it its occupied.
		*/

		for (int i = 0; i < 8; i++) {

			std::cout << 8 - i << ' ';

			for (int j = 0; j < 8; j++) {

				std::uint64_t k = i * 8 + j;

				if (((WP >> k) & 1) == 1) std::cout << 'P';
				else if (((WR >> k) & 1) == 1) std::cout << 'R';
				else if (((WB >> k) & 1) == 1) std::cout << 'B';
				else if (((WQ >> k) & 1) == 1) std::cout << 'Q';
				else if (((WK >> k) & 1) == 1) std::cout << 'K';
				else if (((WN >> k) & 1) == 1) std::cout << 'N';
				else if (((BP >> k) & 1) == 1) std::cout << 'p';
				else if (((BN >> k) & 1) == 1) std::cout << 'n';
				else if (((BR >> k) & 1) == 1) std::cout << 'r';
				else if (((BK >> k) & 1) == 1) std::cout << 'k';
				else if (((BQ >> k) & 1) == 1) std::cout << 'q';
				else if (((BB >> k) & 1) == 1) std::cout << 'b';
				else std::cout << " ";

				std::cout << " ";

			}
			std::cout << '\n';
		}
		std::cout << "  A B C D E F G H\n";

	}

}