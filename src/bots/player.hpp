#include "bot.hpp"

namespace Bots {

	class Player : public BasicBot
	{
	public:
		Player(Game::CubeState _symbol, bool _displayEx = true);

		Game::Turn Step(const Game::Board& _state);

	private:
		const bool m_displayExt;
	};
}