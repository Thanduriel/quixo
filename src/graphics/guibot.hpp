#pragma once

#include "bots/bot.hpp"
#include "renderboard.hpp"

namespace Bots {

	class GUIPlayer : public BasicBot
	{
	public:
		GUIPlayer(Graphics::RenderBoard& _board);

		Game::Turn Step(const Game::Board& _state) override;
		std::string GetName() const override { return "GUI Input"; }

	private:
		Graphics::RenderBoard& m_board;
	};
}