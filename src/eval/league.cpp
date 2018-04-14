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

	void League::Run(int _numGames)
	{
		m_ratings.resize(m_bots.size(), BASE_RATING);

		int n = _numGames / NumThreads;
		auto worker = [this, n, _numGames](const int _tid) {
			for (int i = 0; i < n; ++i)
			{
				const int bId1 = g_random.Uniform(0, static_cast<int>(m_bots.size()) - 1);
				int bId2;
				do {
					bId2 = g_random.Uniform(0, static_cast<int>(m_bots.size()) - 1);
				} while (bId1 == bId2);

				Bots::BasicBot& bot1 = *m_bots[bId1][_tid];
				Bots::BasicBot& bot2 = *m_bots[bId2][_tid];

				GameManager<> manager(bot1, bot2);
				GameResult result = manager.Play();
			//	GameResult result = static_cast<GameResult>(g_random.Uniform(0, 2));

				//update ratings
				float r;
				if (result == GameResult::Cross) r = 1.f;
				else if (result == GameResult::Draw) r = 0.5f;
				else r = 0.f;

				std::unique_lock<std::mutex> lock(m_mutex);
				const float expected = 1.f / (1.f + std::pow(10.f, (m_ratings[bId2] - m_ratings[bId1]) / BASE_RATING));
				const float timeFact = static_cast<float>(i) / n;
				const float k = std::pow(2.71828f, FACT * timeFact);
			//	const float k = (A / (1.f + timeFact) + B);
			//	if (i == _numGames / 3) k = 0.8f;
			//	else if (i == 2 * _numGames / 3) k = 0.2f;
				float update = 16.f * k * (r - expected);

				std::cout << k << std::endl;

				m_ratings[bId1] += update;
				m_ratings[bId2] -= update;
			}
		};

		std::vector<std::thread> threads;
		threads.reserve(NumThreads - 1);
		for (int i = 0; i < NumThreads - 1; ++i)
			threads.emplace_back(worker, i);
		worker(NumThreads-1);

		for (auto& t : threads) t.join();

		// print results
		for (size_t i = 0; i < m_bots.size(); ++i)
		{
			std::cout << m_bots[i][0]->GetName() << "   " << m_ratings[i] << "\n";
		}
	}
}