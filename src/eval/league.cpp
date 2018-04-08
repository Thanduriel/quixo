#include "league.hpp"
#include "game/game.hpp"
#include "utils/random.hpp"
#include <iostream>

namespace Eval {

	using namespace Game;
	using namespace Utils;

	void League::Add(Bots::BasicBot& _bot)
	{
		m_bots.emplace_back(Actor{ &_bot, 400.f });
	}

	void League::Run(int _numGames)
	{
		const float timeMod = _numGames * 1.1f;
		for (int i = 0; i < _numGames; ++i)
		{
			const int bId1 = g_random.Uniform(0, static_cast<int>(m_bots.size()) - 1);
			int bId2;
			do {
				bId2 = g_random.Uniform(0, static_cast<int>(m_bots.size()) - 1);
			} while (bId1 == bId2);

			GameManager<> manager(*m_bots[bId1].bot, *m_bots[bId2].bot);
			GameResult result = manager.Play();

			//update ratings
			float r;
			if (result == GameResult::Cross) r = 1.f;
			else if (result == GameResult::Draw) r = 0.5f;
			else r = 0.f;
			const float expected = 1.f / (1.f + std::pow(10.f, (m_bots[bId1].rating - m_bots[bId2].rating) / 400.f));
			float update = (timeMod -i)/ timeMod * (r - expected);

			m_bots[bId1].rating += update;
			m_bots[bId2].rating -= update;
		}

		// print results
		for (auto& actor : m_bots)
		{
			std::cout << actor.bot->GetName() << "   " << actor.rating << "\n";
		}
	}
}