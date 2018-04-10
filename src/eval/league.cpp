#include "league.hpp"
#include "game/game.hpp"
#include "utils/random.hpp"
#include <iostream>

namespace Eval {

	using namespace Game;
	using namespace Utils;

	constexpr float BASE_RATING = 400.f;
	constexpr float DECAY_MIN = 0.2f;
	//decay function params
	const float FACT = std::log(DECAY_MIN);
	// f(x) = a/(1+x) + b
	constexpr float A = 2.f * (1.f - DECAY_MIN);
	constexpr float B = 1.f - A;

	void League::Add(Bots::BasicBot& _bot)
	{
		m_bots.emplace_back(Actor{ &_bot, BASE_RATING });
	}

	void League::Run(int _numGames)
	{
		float k = 1.5f;
		for (int i = 0; i < _numGames; ++i)
		{
			const int bId1 = g_random.Uniform(0, static_cast<int>(m_bots.size()) - 1);
			int bId2;
			do {
				bId2 = g_random.Uniform(0, static_cast<int>(m_bots.size()) - 1);
			} while (bId1 == bId2);

		//	GameManager<> manager(*m_bots[bId1].bot, *m_bots[bId2].bot);
		//	GameResult result = manager.Play();
			GameResult result = static_cast<GameResult>(g_random.Uniform(0, 2));

			//update ratings
			float r;
			if (result == GameResult::Cross) r = 1.f;
			else if (result == GameResult::Draw) r = 0.5f;
			else r = 0.f;
			const float expected = 1.f / (1.f + std::pow(10.f, (m_bots[bId2].rating - m_bots[bId1].rating) / BASE_RATING));
			const float timeFact = static_cast<float>(i) / _numGames;
			//alternative k function: std::pow(2.71828f, FACT * timeFact)
			//const float k = (A / (1.f + timeFact) + B);
			if (i == _numGames / 3) k = 0.8f;
			else if (i == 2 * _numGames / 3) k = 0.2f;
			float update = 16.f * k * (r - expected);

			std::cout << k << std::endl;

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