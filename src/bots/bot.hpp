#pragma once

#include "game/board.hpp"

namespace Bots {

	class BasicBot
	{
	public:
		BasicBot() = default;

		virtual Game::Turn Step(const Game::Board& _state) = 0;
		virtual std::string GetName() const = 0;
		// called when a game ends
		virtual void FinalizeGame(const Game::Board& _state) {}

		void SetSymbol(Game::CubeState _symbol) { m_symbol = _symbol; m_enemySymbol = GetOther(_symbol); }
		Game::CubeState GetSymbol() const { return m_symbol; }
	protected:
		static Game::CubeState GetOther(Game::CubeState _self)
		{
			using namespace Game;
			return _self == CubeState::Cross ? CubeState::Circle : CubeState::Cross;
		}

		Game::CubeState m_symbol;
		Game::CubeState m_enemySymbol;
	};
}