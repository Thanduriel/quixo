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
		if (m_turnCount >= m_turnStats.size()) return;
		++m_turnStats[m_turnCount][_action.dstX + Board::BoardSize * _action.dstY];
		++m_turnCount;
	}

	void Tracker::PrintHeatMap(size_t turn) const
	{
		for (int y = 0; y < Board::BoardSize; ++y)
		{
			for (int x = 0; x < Board::BoardSize; ++x)
			{
				std::cout << m_turnStats[turn][x + Board::BoardSize * y] << "  ";
			}
			std::cout << "\n";
		}
	}
}