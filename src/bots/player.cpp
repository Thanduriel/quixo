#include "player.hpp"
#include <iostream>
#include <sstream>

namespace Bots {

	Player::Player(Game::CubeState _symbol, bool _displayEx)
		: BasicBot(_symbol),
		m_displayExt(_displayEx)
	{}

	Game::Turn Player::Step(const Game::Board& _state)
	{
		std::cout << "01234" << "\n";
		_state.Print(std::cout);
		std::cout << "Choose action for: " << Game::STATE_CHARS[static_cast<int>(m_symbol)] 
			<< "\n";

		Game::Action action;
		std::string line;
		do {
			std::getline(std::cin, line);
		} while (line.size() != 7);
		std::stringstream ss(line);

		ss >> action.srcX >> action.srcY >> action.dstX >> action.dstY;

		return Game::Turn(action, m_symbol);
	}
}