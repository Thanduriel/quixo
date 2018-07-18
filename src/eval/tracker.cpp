#include "tracker.hpp"
#include <iostream>

namespace Eval {

	using namespace Game;

	Tracker::Tracker(size_t _maxNumTurns)
		: m_turnStats(_maxNumTurns)
	{

	}

	void Tracker::Add(const Game::Action& _action)
	{
		const size_t ind = _action.dstX + Board::BoardSize * _action.dstY;
		if (m_turnCount >= m_turnStats.size())
			++m_aggregatedStats[ind];
		else 
		{
			++m_turnStats[m_turnCount][ind];
			++m_turnCount;
		}
	}

	void Tracker::PrintHeatMap(size_t turn) const
	{
		auto& map = turn < m_turnStats.size() ? m_turnStats[turn] : m_aggregatedStats;
		for (int y = 0; y < Board::BoardSize; ++y)
		{
			for (int x = 0; x < Board::BoardSize; ++x)
			{
				std::cout << map[x + Board::BoardSize * y] << "  ";
			}
			std::cout << "\n";
		}
	}
}