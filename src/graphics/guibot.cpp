#include "guibot.hpp"

namespace Bots {

	GUIPlayer::GUIPlayer(Graphics::RenderBoard& _board)
		: BasicBot(),
		m_board(_board)
	{}

	Game::Turn GUIPlayer::Step(const Game::Board& _state)
	{
		return Game::Turn(m_board.GetInput(_state, m_symbol), m_symbol);
	}
}