#include <iostream>
#include <string>

#include "Board.h"

namespace UCI {

	void getUCI();
	void getSetOption(std::string input);
	void getUCINewGame();
	void getPosition(std::string input);
	void getGo(std::string input);

	/**
	 * .
	 * Invokes the UCI communication protocol.
	 */
	void UCI() {

		while (true) {

			/*
			* Reading a line from the UI. Done through simple input & output
			*/

			std::string ln;
			std::getline(std::cin, ln);

			/*
			* Sets the engine to UCI mode. Move this to the main function later.
			*/
			if (ln == "uci") {
				getUCI();
			}

			/*
			* Sets options of the engine
			*/
			else if (ln.find("setoption") != std::string::npos) {
				getSetOption(ln);
			}

			/*
			* Starts a new game. Resets representation of board.
			*/
			else if (ln == "ucinewgame") {
				getUCINewGame();
			}

			/*
			* Changes the position of the board.
			*/
			else if (ln.find("position") != std::string::npos) {
				getPosition(ln);
			}

			/*
			* Prompts engine for a move.
			*/
			else if (ln.find("go") != std::string::npos) {
				getGo(ln);
			}

			/*
			* Helper command from user to help debug engine
			*/
			else if (ln == "print") {
				Board::printBoard(); 
			}

		}

	}

	/**
	 * .
	 * Makes sure the engine is in UCI mode. Move to main later when adding more communication protocols.
	 */
	void getUCI() {
		
		std::cout << "id name GorillaChess\n";
		std::cout << "id author TheGameMonkey\n";
		std::cout << "uciok\n";

	}

	/**
	 * .
	 * Sets up the options of the engine. Not implemented yet.
	 * \param input
	 */
	void getSetOption(std::string input) {



	}

	/**
	 * .
	 * Resets the representation of the board. Prepares for a new game.
	 */
	void getUCINewGame() {

	}

	/**
	 * .
	 * Changes the position of the board's representation. Can provide moves, a FEN string, or set it to startpos.
	 */
	void getPosition(std::string input) {
		
		if (input.find("startpos") != std::string::npos) {
			Board::loadFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
		}

		else if (input.find("fen") != std::string::npos) {
			Board::loadFEN(input.substr(13));
		}
		else if (input.find("moves") != std::string::npos) {
			//make moves
		}

	}

	/**
	 * .
	 * Sends the best move to the console.
	 * \param input
	 */
	void getGo(std::string input) {
		//get best move
	}

}
