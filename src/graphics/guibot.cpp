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

	void GUIPlayer::FinalizeGame(const Game::Board& _state)
	{
		m_board.ShowEndState(_state, m_symbol);
	}
}