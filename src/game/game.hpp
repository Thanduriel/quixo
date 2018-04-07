#pragma once

#include "board.hpp"

namespace Game{

	template<typename BotT1, typename BotT2, unsigned MaxTurns = 200>
	class GameManager
	{
	public:
		GameManager()
			: m_bot1(CubeState::Cross),
			m_bot2(CubeState::Circle)
		{}

		std::array<int, 3> Play(unsigned _numRounds)
		{
			std::array<int, 3> results = {0};

			for (unsigned i = 0; i < _numRounds; ++i)
				++results[static_cast<int>(Play())];

			return results;
		}

		GameResult Play()
		{
			Board board;

			for (int i = 0; i < MaxTurns; ++i)
			{
				Turn turn = i % 2 ? m_bot1.Step(board) : m_bot2.Step(board);
				// cant use enemy symbol cubes
				assert(board.Get(turn.action.srcX, turn.action.srcY) != (i % 2 ? m_bot2.GetSymbol() : m_bot1.GetSymbol()));
				board.Move(turn.action, turn.state);
				
				const GameResult res = board.Winner();
				if (res != GameResult::None) return  res;
			}
			return GameResult::Draw;
		}

	private:
		BotT1 m_bot1;
		BotT2 m_bot2;
	};
}