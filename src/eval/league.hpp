#pragma once

#include "bots/bot.hpp"
#include <vector>

namespace Eval {

	class League
	{
	public:
		void Add(Bots::BasicBot& _bot);

		void Run(int _numGames);
	private:

		struct Actor
		{
			Bots::BasicBot* bot;
			float rating;
		};
		std::vector<Actor> m_bots;
	};
}