#pragma once

#include <vector>
#include "game/board.hpp"

namespace Eval{

	class Tracker
	{
	public:
		Tracker(size_t _maxNumTurns = 10u);

		void Add(const Game::Action& _action);
		void NextGame() { m_turnCount = 0; }

		void PrintHeatMap(size_t turn) const;
	private:
		template<unsigned BoardSize>
		using HeatMap = std::array<int, BoardSize * BoardSize>;

		size_t m_turnCount = 0;
		std::vector<HeatMap<Game::Board::BoardSize>> m_turnStats;
		HeatMap<Game::Board::BoardSize> m_aggregatedStats;
	};
}