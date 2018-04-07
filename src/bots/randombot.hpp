#pragma once

#include "bot.hpp"
#include "utils/random.hpp"

namespace Bots {

	class RandomBot : public BasicBot
	{
	public:
		using BasicBot::BasicBot;

		Game::Turn Step(const Game::Board<5>& _state)
		{
			using namespace Game;
			const Action* action;
			do{
				action = &Board<5>::ACTIONS[Utils::g_random.Uniform(0, static_cast<int>(Board<5>::ACTIONS.size()) - 1)];
			} while (_state.Get(action->srcX, action->srcY) == m_enemySymbol);

			return Turn(*action, m_symbol);
		}
	};
}