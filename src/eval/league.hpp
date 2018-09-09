#pragma once

#include "bots/bot.hpp"
#include <vector>
#include <mutex>
#include <memory>

namespace Eval {

	constexpr int NumThreads = 3;

	class League
	{
	public:
		League(){}

		template<typename T, typename =std::enable_if_t<std::is_base_of_v<Bots::BasicBot, T>>>
		void Add(T& _bot)
		{
		/*	BotSet set;
			set.reserve(NumThreads);
			for (int i = 0; i < NumThreads; ++i)
				set.emplace_back(new T(_bot));
			m_bots.emplace_back(std::move(set));*/		
		}

		void Run(int _numGames);
	private:
		using BotSet = std::vector<std::unique_ptr<Bots::BasicBot>>;
		std::vector<BotSet> m_bots;
		std::vector<float> m_ratings;
		std::mutex m_mutex;
	};
}