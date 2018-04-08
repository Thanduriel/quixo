#include "bot.hpp"

namespace Bots {

	class Player : public BasicBot
	{
	public:
		Player(bool _displayEx = true);

		Game::Turn Step(const Game::Board& _state) override;
		std::string GetName() const override { return "Input"; }

	private:
		const bool m_displayExt;
	};
}