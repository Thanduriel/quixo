#pragma once

#include "board.hpp"
#include "bots/bot.hpp"
#include "eval/tracker.hpp"
#include <iostream>

namespace Game{

	enum struct PrintMode
	{
		AllStates,
		EndState,
		None
	};

	template<unsigned MaxTurns = 200, bool UseTracker = true>
	class GameManager
	{
	public:
		GameManager(Bots::BasicBot& _bot1, Bots::BasicBot& _bot2, PrintMode _printMode = PrintMode::None)
			: m_bot1(_bot1),
			m_bot2(_bot2),
			m_printMode(_printMode)
		{
			m_bot1.SetSymbol(CubeState::Cross);
			m_bot2.SetSymbol(CubeState::Circle);
		}

		std::array<int, 3> Play(unsigned _numRounds)
		{
			std::array<int, 3> results = { 0 };

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
				assert(board.Get(turn.action.srcX, turn.action.srcY) 
					!= (i % 2 ? m_bot2.GetSymbol() : m_bot1.GetSymbol()));

				board.Move(turn.action, turn.state);
				if constexpr(UseTracker) m_tracker.Add(turn.action);

				if (m_printMode == PrintMode::AllStates)
				{
					board.Print(std::cout);
					std::cout << "\n";
				}
				const GameResult res = board.Winner();
				if (res != GameResult::None)
				{
					if constexpr(UseTracker) m_tracker.NextGame();
					if (m_printMode == PrintMode::EndState) 
					{ 
						board.Print(std::cout); 
						std::cout << "\n"; 
					}
					return  res;
				}
			}
			return GameResult::Draw;
		}

		const Eval::Tracker& GetTracker() { return m_tracker; }

	private:
		Bots::BasicBot& m_bot1;
		Bots::BasicBot& m_bot2;
		PrintMode m_printMode;
		Eval::Tracker m_tracker;
	};
}