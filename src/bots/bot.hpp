#pragma once

#include "game/board.hpp"

namespace Bots {

	class BasicBot
	{
	public:
		BasicBot(Game::CubeState _symbol) : m_symbol(_symbol), m_enemySymbol(GetOther(_symbol)) {}

	protected:
		Game::CubeState GetOther(Game::CubeState _self) const
		{
			using namespace Game;
			return _self == CubeState::Cross ? CubeState::Circle : CubeState::Cross;
		}

		Game::CubeState m_symbol;
		Game::CubeState m_enemySymbol;
	};
}