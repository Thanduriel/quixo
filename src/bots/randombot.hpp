#pragma once

#include "bot.hpp"
#include "utils/random.hpp"

namespace Bots {

	class RandomBot : public BasicBot
	{
	public:
		using BasicBot::BasicBot;

		Game::Turn Step(const Game::Board& _state) override
		{
			using namespace Game;
			const Action* action;
			do{
				action = &Board::ACTIONS[Utils::g_random.Uniform(0, static_cast<int>(Board::ACTIONS.size()) - 1)];
			} while (_state.Get(action->srcX, action->srcY) == m_enemySymbol);

			return Turn(*action, m_symbol);
		}

		std::string GetName() const override { return "Random"; }
	};
}